#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <iostream>
#include <mutex>

#include "core/RayTracer.hpp"
#include "core/Timer.hpp"

#include <sampling/Sampler.hpp>
#include <sampling/Independent.hpp>

namespace
{
    const int DefaultImageWidth = 512;
    const int DefaultImageHeight = 512;
    const int BlockSize = 50;
}

namespace vesp
{
    RayTracer::RayTracer()
        : m_image(Vector2i(DefaultImageWidth, DefaultImageHeight))
        , m_renderStatus(RenderStatus::Done)
        , m_progress(1.f)
        , m_scene(nullptr)
    {
        // Recommended for embree before thread creation
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    }

    RayTracer::~RayTracer()
    {
        // stop rendering
        //if (m_render)

        delete m_scene;
    }

    void RayTracer::initializeScene(const std::string& filename)
    {
        // load scene from the file, etc.

        int width = DefaultImageWidth;
        int height = DefaultImageHeight;

        m_image.initialize(Vector2i(width, height));
        m_image.clear();

        // determine output etc.

        m_scene = new Scene(width, height);

        sceneInitialized(width, height);
    }

    void RayTracer::startRayTracing()
    {
        m_renderStatus = RenderStatus::Busy;
        m_renderThread = std::thread([this]
        {
            Vector2i size = m_image.getSize();
            int width = size.x();
            int height = size.y();

            generateImageBlocks(width, height);

            int numBlocks = static_cast<int>(m_blockQueue.unsafe_size());
            int numRows = (height - 1) / BlockSize + 1;
            int numCols = (width - 1) / BlockSize + 1;

            std::cout << "Using " << tbb::task_scheduler_init::default_num_threads() << " threads." << std::endl;

            tbb::concurrent_vector<std::unique_ptr<Sampler>> samplers(numBlocks);

#ifdef _DEBUG
            tbb::task_scheduler_init init(1);
#endif

            for (auto& it : samplers)
                it = std::make_unique<IndependentSampler>();

            auto renderImageBlocks = [&](const tbb::blocked_range<int>& range)
            {

                for (int i = range.begin(); i < range.end(); ++i)
                {
                    RayTracer::ImageBlockDescriptor currIbd(0, 0, 0, 0);

                    if (m_blockQueue.try_pop(currIbd))
                    {
                        ImageBlock currentBlock(Vector2i(currIbd.w, currIbd.h));

                        int blockId = (currIbd.y / BlockSize) * numCols + currIbd.x / BlockSize;

                        renderBlock(currentBlock, currIbd, *samplers.at(blockId).get(), m_scene);

                        updateImage(currentBlock, currIbd.x, currIbd.y);
                    }
                }
            };

            tbb::blocked_range<int> range(0, numBlocks);

            NanoTimer timer;

            tbb::parallel_for(range, renderImageBlocks);

            double ms = static_cast<double>(timer.getElapsedTime().count()) / 1'000'000;

            std::cout << "Finished rendering scene. Computed in " << ms << " ms. " << std::endl;
        });

        m_renderThread.detach();
    }

    void RayTracer::generateImageBlocks(int width, int height)
    {
        m_blockQueue.clear();

        int currRow = 0;
        int currCol = 0;
        while (currRow < height)
        {
            int currHeight = BlockSize;
            if (currRow + BlockSize > height) currHeight = height - currRow;

            currCol = 0;
            while (currCol < width)
            {
                int currWidth = BlockSize;
                if (currCol + BlockSize > width) currWidth = width - currCol;

                m_blockQueue.push(ImageBlockDescriptor(currCol, currRow, currWidth, currHeight));

                currCol += BlockSize;
            }

            currRow += BlockSize;
        }
    }

    void RayTracer::renderBlock(ImageBlock& block, RayTracer::ImageBlockDescriptor& blockDesc, Sampler& sampler, const Scene* scene)
    {
        //std::default_random_engine eng(std::random_device{}());
        //std::uniform_real_distribution<float> distrib(0, 1);
        //std::uniform_int_distribution<int> distribi(1, 5);

        //float sum = 0.f;
        //for (int i = 0; i < 1000'000; i++)
        //    sum += distrib(eng);

        // getCamera
        // getIntegrator

        const Camera* camera = scene->getCamera();

        std::vector<Spectrum> colors;
        colors.push_back({ 1.f, 1.f, 0.f });
        colors.push_back({ 0.f, 0.f, 1.f });
        colors.push_back({ 1.f, 0.f, 1.f });
        colors.push_back({ 0.f, 1.f, 0.f });
        

        block.clear();

        Point2i offset(blockDesc.x, blockDesc.y);

        for (int y = 0; y < block.getSize().y(); ++y)
        {
            for (int x = 0; x < block.getSize().x(); ++x)
            {
                float px = x + blockDesc.x + sampler.next1D();
                float py = y + blockDesc.y + sampler.next1D();

                Point2f apertureSample = sampler.next2D();

                Ray3f ray;

                Spectrum response = camera->sampleRay(ray, Point2f(px, py), apertureSample);

                Intersection its;
                if (scene->rayIntersect(ray, its))
                {
                    block(y, x) = colors[2 * its.geomId + its.triId % 2];
                }
                else
                    block(y, x) = Spectrum(px / DefaultImageWidth, py / DefaultImageHeight, 1.f);

                //std::cout << "Pixel: " << px << " : " << py << std::endl;
            }
        }
    }

    void RayTracer::updateImage(ImageBlock& imageBlock, int xOffset, int yOffset)
    {
        std::lock_guard<std::mutex> lock(m_imageMutex);

        imageUpdated(imageBlock, xOffset, yOffset);
    }
}
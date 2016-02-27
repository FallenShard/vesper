#include <iostream>
#include <xmmintrin.h>
#include <pmmintrin.h>

#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include <core/RayTracer.hpp>
#include <core/Scene.hpp>
#include <core/Timer.hpp>
#include <core/VesperException.hpp>
#include <core/VesperObjectFactory.hpp>

#include <sampling/Sampler.hpp>
#include <sensors/Sensor.hpp>
#include <integrators/Integrator.hpp>

#include <io/FileParser.hpp>

namespace
{
    const int DefaultImageWidth = 800;
    const int DefaultImageHeight = 600;
    const int BlockSize = 50;
}

namespace vesp
{
    RayTracer::RayTracer()
        : m_image(Vector2i(DefaultImageWidth, DefaultImageHeight))
        , m_renderStatus(RenderStatus::Free)
        , m_progress(1.f)
        , m_scene(nullptr)
    {
        // Recommended for embree before thread creation
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

        VesperObjectFactory::initialize();
    }

    RayTracer::~RayTracer()
    {
        stopRayTracing();
    }

    void RayTracer::initializeScene(const std::string& filename)
    {
        if (m_renderStatus == RenderStatus::Busy)
            return;

        try
        {
            FileParser parser;
            m_scene = parser.loadFromXml(filename);

            auto imageSize = m_scene->getSensor()->getImageSize();
            sceneInitialized(imageSize.x(), imageSize.y());

            m_image.initialize(imageSize);
            m_image.clear();
        }
        catch (VesperException ex)
        {
            std::cerr << ex << std::endl;
            m_scene = nullptr;
        }
    }

    void RayTracer::startRayTracing()
    {
        if (!m_scene || m_renderStatus == RenderStatus::Busy)
            return;

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
                it = m_scene->getSampler()->clone();

            auto renderImageBlocks = [&](const tbb::blocked_range<int>& range)
            {
                for (int i = range.begin(); i < range.end(); ++i)
                {
                    if (m_renderStatus == RenderStatus::Interrupted)
                        break;

                    RayTracer::ImageBlockDescriptor currIbd;

                    if (m_blockQueue.try_pop(currIbd))
                    {
                        ImageBlock currentBlock(Vector2i(currIbd.w, currIbd.h));

                        int blockId = (currIbd.y / BlockSize) * numCols + currIbd.x / BlockSize;

                        renderBlock(currentBlock, currIbd, *samplers.at(blockId).get(), m_scene.get());

                        updateImage(currentBlock, currIbd.x, currIbd.y);
                    }
                }
            };

            tbb::blocked_range<int> range(0, numBlocks);

            NanoTimer timer;

            const_cast<Integrator*>(m_scene->getIntegrator())->preprocess(m_scene.get());

            tbb::parallel_for(range, renderImageBlocks);

            double ms = static_cast<double>(timer.getElapsedTime().count()) / 1'000'000;

            std::cout << "Finished rendering scene. Computed in " << ms << " ms. " << std::endl;

            if (m_renderStatus != RenderStatus::Interrupted)
                m_renderThread.detach();

            m_renderStatus = RenderStatus::Done;
        });
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

        m_blocksRendered = 0;
        m_totalBlocks = static_cast<int>(m_blockQueue.unsafe_size());
    }

    void RayTracer::renderBlock(ImageBlock& block, RayTracer::ImageBlockDescriptor& blockDesc, Sampler& sampler, const Scene* scene)
    {
        block.clear();

        const Sensor* sensor = scene->getSensor();
        const Integrator* integrator = scene->getIntegrator();
        size_t numSamples = sampler.getSampleCount();

        for (int y = 0; y < block.getSize().y(); ++y)
        {
            for (int x = 0; x < block.getSize().x(); ++x)
            {
                float weightSum = 0.f;
                for (int s = 0; s < numSamples; s++)
                {
                    Point2f pixelSample(x + blockDesc.x + sampler.next1D(), y + blockDesc.y + sampler.next1D());
                    Point2f apertureSample = sampler.next2D();

                    Ray3f ray;
                    Spectrum response = sensor->sampleRay(ray, pixelSample, apertureSample);

                    block(y, x) += response * integrator->Li(scene, sampler, ray);
                }

                block(y, x) /= static_cast<float>(numSamples);
            }
        }
    }

    void RayTracer::updateImage(ImageBlock& imageBlock, int xOffset, int yOffset)
    {
        std::lock_guard<std::mutex> lock(m_imageMutex);

        m_blocksRendered++;
        std::cout << "Blocks rendered: " << m_blocksRendered << " / " << m_totalBlocks << std::endl;

        imageUpdated(imageBlock, xOffset, yOffset);
    }

    void RayTracer::stopRayTracing()
    {
        if (m_renderStatus == RenderStatus::Busy)
        {
            m_renderStatus = RenderStatus::Interrupted;
            m_renderThread.join();
            m_renderStatus = RenderStatus::Free;
            std::cout << "Rendering cancelled." << std::endl;
        }
    }
}
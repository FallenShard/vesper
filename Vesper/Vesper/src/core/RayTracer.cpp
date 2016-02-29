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
    const int BlockSize = 32;
}

namespace vesp
{
    RayTracer::RayTracer()
        : m_renderStatus(RenderStatus::Free)
        , m_progress(1.f)
        , m_scene(nullptr)
    {
        // Recommended for embree before thread creation
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

        VesperObjectFactory::initialize();
        m_image.initialize(Vector2i(DefaultImageWidth, DefaultImageHeight), nullptr);
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

            m_image.initialize(imageSize, m_scene->getSensor()->getReconstructionFilter());
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

                    ImageBlock::Descriptor desc;

                    if (m_blockQueue.try_pop(desc))
                    {
                        ImageBlock currentBlock(desc.offset, desc.size, m_scene->getSensor()->getReconstructionFilter());

                        int blockId = (desc.offset.y() / BlockSize) * numCols + desc.offset.x() / BlockSize;

                        renderBlock(currentBlock, *samplers.at(blockId), m_scene.get());

                        updateImage(currentBlock);
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
        int numRows = (height - 1) / BlockSize + 1;
        int numCols = (width - 1) / BlockSize + 1;

        std::vector<ImageBlock::Descriptor> descriptors;
        int currRow = 0;
        int currCol = 0;
        while (currRow < height)
        {
            int currHeight = BlockSize;
            if (currRow + BlockSize > height) currHeight = height - currRow;

            int currBlockRow = (currHeight - 1) / BlockSize;

            currCol = 0;
            while (currCol < width)
            {
                int currWidth = BlockSize;
                if (currCol + BlockSize > width) currWidth = width - currCol;

                int currBlockCol = (currWidth - 1) / BlockSize;

                ImageBlock::Descriptor desc(currCol, currRow, currWidth, currHeight);
                descriptors.push_back(desc);

                currCol += BlockSize;
            }

            currRow += BlockSize;
        }

        std::vector<std::vector<unsigned int>> indexMat(numRows, std::vector<unsigned>(numCols));
        for (int i = 0; i < numRows; ++i)
            for (int j = 0; j < numCols; ++j)
                indexMat[i][j] = i * numCols + j;

        std::vector<unsigned int> indices;

        int i;
        int k = 0;
        int l = 0;
        int m = numRows;
        int n = numCols;

        while (k < m && l < n)
        {
            for (i = l; i < n; ++i)
                indices.push_back(indexMat[k][i]);
            k++;
            for (i = k; i < m; ++i)
                indices.push_back(indexMat[i][n - 1]);
            n--;

            if (k < m)
            {
                for (i = n - 1; i >= l; --i)
                    indices.push_back(indexMat[m - 1][i]);
                m--;
            }

            if (l < n)
            {
                for (i = m - 1; i >= k; --i)
                    indices.push_back(indexMat[i][l]);
                l++;
            }
        }

        for (auto i = 0; i < indices.size() / 2; ++i)
            std::swap(indices[i], indices[indices.size() - 1 - i]);

        for (auto idx : indices)
            m_blockQueue.push(descriptors[idx]);

        m_blocksRendered = 0;
        m_totalBlocks = static_cast<int>(m_blockQueue.unsafe_size());
    }

    void RayTracer::renderBlock(ImageBlock& block, Sampler& sampler, const Scene* scene)
    {
        block.clear();
        Point2i offset = block.getOffset();

        const Sensor* sensor = scene->getSensor();
        const Integrator* integrator = scene->getIntegrator();
        size_t numSamples = sampler.getSampleCount();
        sampler.prepare();

        for (int y = 0; y < block.getFullSize().y(); ++y)
        {
            for (int x = 0; x < block.getFullSize().x(); ++x)
            {
                float weightSum = 0.f;
                for (int s = 0; s < numSamples; s++)
                {
                    Point2f pixelSample(x + offset.x() + sampler.next1D(), y + offset.y() + sampler.next1D());
                    Point2f apertureSample = sampler.next2D();

                    Ray3f ray;
                    Spectrum response = sensor->sampleRay(ray, pixelSample, apertureSample);
                    Spectrum radiance = integrator->Li(scene, sampler, ray);
                    block.addSample(pixelSample, response * radiance);
                }
            }
        }
    }

    void RayTracer::updateImage(ImageBlock& imageBlock)
    {
        std::lock_guard<std::mutex> lock(m_imageMutex);

        m_blocksRendered++;
        std::cout << "Blocks rendered: " << m_blocksRendered << " / " << m_totalBlocks << std::endl;

        imageUpdated(imageBlock);
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
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
#include <random>

#include "core/RayTracer.hpp"
#include "core/Timer.hpp"

#include <iostream>
#include <mutex>

namespace
{
    const int DefaultImageWidth = 600;
    const int DefaultImageHeight = 400;
    const int BlockSize = 50;
}

RayTracer::RayTracer()
    : m_image(Vector2i(DefaultImageWidth, DefaultImageHeight))
    , m_renderStatus(RenderStatus::Done)
    , m_progress(1.f)
{
}

RayTracer::~RayTracer()
{
    // stop rendering
    //if (m_render)
}

void RayTracer::initializeScene(const std::string& filename)
{
    // load scene from the file, etc.

    int width = DefaultImageWidth;
    int height = DefaultImageHeight;

    m_image.initialize(Vector2i(width, height));
    m_image.clear();

    // determine output etc.

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

        std::cout << "Using " << tbb::task_scheduler_init::default_num_threads() << " threads." << std::endl;

        auto renderImageBlocks = [&](const tbb::blocked_range<int>& range)
        {

            for (int i = range.begin(); i < range.end(); ++i)
            {
                RayTracer::ImageBlockDescriptor currIbd(0, 0, 0, 0);

                if (m_blockQueue.try_pop(currIbd))
                {
                    ImageBlock currentBlock(Vector2i(currIbd.w, currIbd.h));
                    renderBlock(currentBlock, currIbd);

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

void RayTracer::renderBlock(ImageBlock& block, RayTracer::ImageBlockDescriptor& blockDesc)
{
    std::default_random_engine eng(std::random_device{}());
    std::uniform_real_distribution<float> distrib(0, 1);
    std::uniform_int_distribution<int> distribi(1, 5);

    float sum = 0.f;
    for (int i = 0; i < 1000'000; i++)
        sum += distrib(eng);

    Spectrum spec(distrib(eng), distrib(eng), distrib(eng));

    for (int i = 0; i < block.getSize().y(); i++)
    {
        for (int j = 0; j < block.getSize().x(); j++)
        {
            block(i, j) = spec;
        }
    }
}

void RayTracer::updateImage(ImageBlock& imageBlock, int xOffset, int yOffset)
{
    std::lock_guard<std::mutex> lock(m_imageMutex);

    imageUpdated(imageBlock, xOffset, yOffset);
}

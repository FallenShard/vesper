#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include <tbb/concurrent_queue.h>

#include <core/ImageBlock.hpp>
#include <core/EventSource.hpp>
#include <core/Renderer.hpp>

namespace vesp
{
    class RayTracer
    {
    public:
        RayTracer();
        ~RayTracer();

        void initializeScene(const std::string& filename);
        void startRayTracing();

        void updateImage(ImageBlock& imageBlock, int xOffset, int yOffset);

        EventSource<void, int, int> sceneInitialized;
        EventSource<void, ImageBlock&, int, int> imageUpdated;

    private:
        struct ImageBlockDescriptor
        {
            int x, y, w, h;
            ImageBlockDescriptor(int xx, int yy, int ww, int hh) : x(xx), y(yy), w(ww), h(hh) {}
            ImageBlockDescriptor() : x(0), y(0) {}
        };

        void generateImageBlocks(int width, int height);
        void renderBlock(ImageBlock& block, ImageBlockDescriptor& blockDesc);

        ImageBlock m_image;

        tbb::concurrent_queue<ImageBlockDescriptor> m_blockQueue;

        enum class RenderStatus
        {
            Done,
            Busy
        };

        std::thread m_renderThread;
        std::atomic<RenderStatus> m_renderStatus;
        std::atomic<float> m_progress;
        std::mutex m_imageMutex;
    };
}
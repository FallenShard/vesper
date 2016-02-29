#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include <tbb/concurrent_queue.h>

#include <core/ImageBlock.hpp>
#include <core/EventSource.hpp>

namespace vesp
{
    class Sampler;
    class Scene;

    class RayTracer
    {
    public:
        RayTracer();
        ~RayTracer();

        void initializeScene(const std::string& filename);
        void startRayTracing();

        void updateImage(ImageBlock& imageBlock);

        void stopRayTracing();

        EventSource<void, int, int> sceneInitialized;
        EventSource<void, ImageBlock&> imageUpdated;

    private:
        void generateImageBlocks(int width, int height);
        void renderBlock(ImageBlock& block, Sampler& sampler, const Scene* scene);

        std::shared_ptr<Scene> m_scene;
        ImageBlock m_image;

        tbb::concurrent_queue<ImageBlock::Descriptor> m_blockQueue;

        enum class RenderStatus
        {
            Free,
            Busy,
            Interrupted,
            Done
        };

        std::thread m_renderThread;
        std::atomic<RenderStatus> m_renderStatus;
        std::atomic<float> m_progress;
        std::mutex m_imageMutex;
        int m_blocksRendered;
        int m_totalBlocks;
    };
}
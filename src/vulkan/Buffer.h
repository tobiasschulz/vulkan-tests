//
// Created by tobias on 11.02.17.
//

#ifndef VULKANTEST_BUFFER_H
#define VULKANTEST_BUFFER_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "Renderer.h"
#include "helper/BufferHelper.h"

namespace vulkan
{

    class Buffer
    {
    public:
        Buffer (vulkan::Renderer *renderer, vk::BufferUsageFlagBits bufferUsage);

        void create (vk::DeviceSize bufferSize, const void *data);
        void update (vk::DeviceSize bufferSize, const void *data);

        vk::Buffer getBuffer ();
        vk::DeviceSize getBufferSize ();

    protected:
        vulkan::Renderer *renderer;

    private:
        vk::BufferUsageFlagBits bufferUsage;
        vk::DeviceSize bufferSize;

        std::shared_ptr<vk::UniqueBuffer> stagingBuffer;
        std::shared_ptr<vk::UniqueDeviceMemory> stagingBufferMemory;
        std::shared_ptr<vk::UniqueBuffer> gpuBuffer;
        std::shared_ptr<vk::UniqueDeviceMemory> gpuBufferMemory;
    };

}

#endif //VULKANTEST_BUFFER_H

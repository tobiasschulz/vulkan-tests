//
// Created by tobias on 05.02.17.
//

#include "UniformBuffer.h"
#include <chrono>

namespace helper
{

    UniformBuffer::UniformBuffer (helper::Renderer &_renderer)
            : renderer (_renderer)
    {

    }

    void UniformBuffer::create ()
    {
        vk::DeviceSize bufferSize = sizeof (UniformBufferObject);

        std::tie (uniformStagingBuffer, uniformStagingBufferMemory) = helper::BufferHelper::createBuffer (
                renderer,
                bufferSize,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        std::tie (uniformBuffer, uniformBufferMemory) = helper::BufferHelper::createBuffer (
                renderer,
                bufferSize,
                vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eDeviceLocal
        );


    }

    void UniformBuffer::update ()
    {
        static auto startTime = std::chrono::high_resolution_clock::now ();

        auto currentTime = std::chrono::high_resolution_clock::now ();
        float time = std::chrono::duration_cast<std::chrono::milliseconds> (currentTime - startTime).count () / 1000.0f;

        UniformBufferObject ubo = {};
        ubo.model = glm::rotate (glm::mat4 (), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective (glm::radians (45.0f), renderer.getSwapChainExtent ().width / (float) renderer.getSwapChainExtent ().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void *data = renderer.getDevice ().mapMemory (*uniformStagingBufferMemory, 0, sizeof (ubo));
        memcpy (data, &ubo, sizeof (ubo));
        renderer.getDevice ().unmapMemory (*uniformStagingBufferMemory);

        helper::BufferHelper::copyBuffer (renderer, *uniformStagingBuffer, *uniformBuffer, sizeof (ubo));
    }

}

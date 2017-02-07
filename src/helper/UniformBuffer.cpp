//
// Created by tobias on 05.02.17.
//

#include "UniformBuffer.h"

namespace helper
{

    UniformBuffer::UniformBuffer ()
    {
    }

    void UniformBuffer::create (helper::Renderer *renderer)
    {
        if (renderer == nullptr) return;

        vk::DeviceSize bufferSize = sizeof (UniformBufferObject);

        vk::Buffer _uniformStagingBuffer;
        vk::DeviceMemory _uniformStagingBufferMemory;
        std::tie (_uniformStagingBuffer, _uniformStagingBufferMemory) = helper::BufferHelper::createBuffer (
                *renderer,
                bufferSize,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
        );
        uniformStagingBuffer = vk::UniqueBuffer (_uniformStagingBuffer);
        uniformStagingBufferMemory = vk::UniqueDeviceMemory (_uniformStagingBufferMemory);


        vk::Buffer _uniformBuffer;
        vk::DeviceMemory _uniformBufferMemory;
        std::tie (_uniformBuffer, _uniformBufferMemory) = helper::BufferHelper::createBuffer (
                *renderer,
                bufferSize,
                vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eUniformBuffer,
                vk::MemoryPropertyFlagBits::eDeviceLocal
        );
        uniformBuffer = vk::UniqueBuffer (_uniformBuffer);
        uniformBufferMemory = vk::UniqueDeviceMemory (_uniformBufferMemory);


    }

    void UniformBuffer::update (helper::Renderer *renderer, helper::UniformBufferObject ubo)
    {
        if (renderer == nullptr) return;
        auto device = renderer->getDevice ();

        void *data = device.mapMemory (*uniformStagingBufferMemory, 0, sizeof (ubo));
        memcpy (data, &ubo, sizeof (ubo));
        device.unmapMemory (*uniformStagingBufferMemory);

        helper::BufferHelper::copyBuffer (*renderer, *uniformStagingBuffer, *uniformBuffer, sizeof (ubo));
    }

}

//
// Created by tobias on 11.02.17.
//

#include "Buffer.h"


namespace vulkan
{

    Buffer::Buffer (vulkan::Renderer *_renderer, vk::BufferUsageFlagBits _bufferUsage)
            : renderer (_renderer), bufferUsage (_bufferUsage)
    {
    }

    void Buffer::create (vk::DeviceSize bufferSize, const void *data)
    {
        auto device = renderer->getDevice ();
        this->bufferSize = bufferSize;

        // create staging buffer
        std::tie (stagingBuffer, stagingBufferMemory) = helper::BufferHelper::createBufferShared (
                renderer,
                bufferSize,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        // copy data to staging buffer
        void *_data = device.mapMemory (**stagingBufferMemory, 0, bufferSize);
        memcpy (_data, data, (size_t) bufferSize);
        device.unmapMemory (**stagingBufferMemory);

        // create final buffer
        std::tie (gpuBuffer, gpuBufferMemory) = helper::BufferHelper::createBufferShared (
                renderer,
                bufferSize,
                vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | bufferUsage, // bufferUsage = vertex buffer, index buffer, uniform buffer?
                vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        helper::BufferHelper::copyBuffer (renderer, **stagingBuffer, **gpuBuffer, bufferSize);

    }

    void Buffer::update (vk::DeviceSize bufferSize, const void *data)
    {
        auto device = renderer->getDevice ();

        void *_data = device.mapMemory (**stagingBufferMemory, 0, bufferSize);
        memcpy (_data, data, (size_t) bufferSize);
        device.unmapMemory (**stagingBufferMemory);

        helper::BufferHelper::copyBuffer (renderer, **stagingBuffer, **gpuBuffer, bufferSize);
    }

    vk::Buffer Buffer::getBuffer ()
    {
        return **gpuBuffer;
    }

    vk::DeviceSize Buffer::getBufferSize ()
    {
        return bufferSize;
    }

}

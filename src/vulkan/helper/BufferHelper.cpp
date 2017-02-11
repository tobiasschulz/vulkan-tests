//
// Created by tobias on 06.02.17.
//

#include "BufferHelper.h"

namespace helper
{


    std::pair<std::shared_ptr<vk::UniqueBuffer>, std::shared_ptr<vk::UniqueDeviceMemory>>
    BufferHelper::createBufferShared (vulkan::Renderer *renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::Buffer _indexBuffer;
        vk::DeviceMemory _indexBufferMemory;
        std::tie (_indexBuffer, _indexBufferMemory) = createBuffer (renderer, size, usage, properties);
        return std::make_pair (std::make_shared<vk::UniqueBuffer> (_indexBuffer), std::make_shared<vk::UniqueDeviceMemory> (_indexBufferMemory));
    }

    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory>
    BufferHelper::createBufferUnique (vulkan::Renderer *renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::Buffer _indexBuffer;
        vk::DeviceMemory _indexBufferMemory;
        std::tie (_indexBuffer, _indexBufferMemory) = createBuffer (renderer, size, usage, properties);
        return std::make_pair (vk::UniqueBuffer (_indexBuffer), vk::UniqueDeviceMemory (_indexBufferMemory));
    }

    std::pair<vk::Buffer, vk::DeviceMemory> BufferHelper::createBuffer (vulkan::Renderer *renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::Device device = renderer->getDevice ();

        vk::BufferCreateInfo bufferInfo;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        vk::Buffer buffer = device.createBuffer (bufferInfo);

        vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements (buffer);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = renderer->findMemoryType (memRequirements.memoryTypeBits, properties);

        vk::DeviceMemory bufferMemory = device.allocateMemory (allocInfo);
        device.bindBufferMemory (buffer, bufferMemory, 0);

        return std::make_pair (buffer, bufferMemory);
    }

    void BufferHelper::copyBuffer (vulkan::Renderer *renderer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBuffer commandBuffer = renderer->beginSingleTimeCommands ();

        vk::BufferCopy copyRegion;
        copyRegion.size = size;

        commandBuffer.copyBuffer (srcBuffer, dstBuffer, 1, &copyRegion);

        renderer->endSingleTimeCommands (commandBuffer);
    }


}
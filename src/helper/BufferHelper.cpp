//
// Created by tobias on 06.02.17.
//

#include "BufferHelper.h"

namespace helper
{


    std::pair<vk::Buffer, vk::DeviceMemory> BufferHelper::createBuffer (helper::Renderer &renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::Device device = renderer.getDevice ();

        vk::BufferCreateInfo bufferInfo;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        vk::Buffer buffer = device.createBuffer (bufferInfo);

        vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements (buffer);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = renderer.findMemoryType (memRequirements.memoryTypeBits, properties);

        vk::DeviceMemory bufferMemory = device.allocateMemory (allocInfo);
        device.bindBufferMemory (buffer, bufferMemory, 0);

        return std::make_pair (buffer, bufferMemory);
    }

    void BufferHelper::copyBuffer (Renderer &renderer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBuffer commandBuffer = renderer.beginSingleTimeCommands ();

        vk::BufferCopy copyRegion;
        copyRegion.size = size;

        commandBuffer.copyBuffer (srcBuffer, dstBuffer, 1, &copyRegion);

        renderer.endSingleTimeCommands (commandBuffer);
    }


}
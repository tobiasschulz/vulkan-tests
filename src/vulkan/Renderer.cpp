//
// Created by tobias on 06.02.17.
//

#include "Renderer.h"


namespace vulkan
{

    vk::Device Renderer::getDevice ()
    {
        return device.get ();
    }

    vk::Queue Renderer::getGraphicsQueue ()
    {
        return graphicsQueue;
    }

    vk::Queue Renderer::getPresentQueue ()
    {
        return presentQueue;
    }

    vk::CommandPool Renderer::getCommandPool ()
    {
        return commandPool.get ();
    }

    vk::Extent2D Renderer::getSwapChainExtent ()
    {
        return swapChainExtent;
    }

    uint32_t Renderer::findMemoryType (uint32_t typeFilter, vk::MemoryPropertyFlags properties)
    {
        vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties ();

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error ("failed to find suitable memory type!");
    }


    vk::CommandBuffer Renderer::beginSingleTimeCommands ()
    {
        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = *commandPool;
        allocInfo.commandBufferCount = 1;

        std::vector<vk::CommandBuffer> _buffers = device->allocateCommandBuffers (allocInfo);
        vk::CommandBuffer commandBuffer = _buffers.front ();

        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        commandBuffer.begin (beginInfo);

        return commandBuffer;
    }

    void Renderer::endSingleTimeCommands (vk::CommandBuffer commandBuffer)
    {
        commandBuffer.end ();

        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        graphicsQueue.submit (1, &submitInfo, vk::Fence ());
        graphicsQueue.waitIdle ();

        device->freeCommandBuffers (*commandPool, 1, &commandBuffer);
    }

}

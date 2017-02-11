//
// Created by tobias on 06.02.17.
//

#ifndef VULKANTEST_RENDERER_H
#define VULKANTEST_RENDERER_H

#include "vulkan.h"
#include "glm/glm.hpp"

namespace vulkan
{

    class Renderer
    {
    public:
        uint32_t findMemoryType (uint32_t typeFilter, vk::MemoryPropertyFlags properties);
        vk::CommandBuffer beginSingleTimeCommands ();
        void endSingleTimeCommands (vk::CommandBuffer commandBuffer);

        vk::Device getDevice ();
        vk::Queue getGraphicsQueue ();
        vk::Queue getPresentQueue ();
        vk::CommandPool getCommandPool ();
        vk::Extent2D getSwapChainExtent ();

    protected:
        vk::Extent2D swapChainExtent;

        vk::PhysicalDevice physicalDevice;
        vk::UniqueDevice device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::UniqueCommandPool commandPool;

    };

}

#endif //VULKANTEST_RENDERER_H

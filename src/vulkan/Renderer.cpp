//
// Created by tobias on 06.02.17.
//

#include <set>

#include "Renderer.h"
#include "helper/SwapChainSupport.h"
#include "helper/QueueFamilies.h"
#include "Surface.h"

const std::vector<const char *> REQUIRED_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };

const std::vector<const char *> VALIDATION_LAYERS = {
        "VK_LAYER_LUNARG_standard_validation",
        // "VK_LAYER_LUNARG_api_dump",
};

namespace vulkan
{

    Renderer::Renderer (bool _enableValidationLayers)
            : enableValidationLayers (_enableValidationLayers),
              validationLayers (VALIDATION_LAYERS),
              requiredExtensions (REQUIRED_EXTENSIONS),
              window (std::make_unique<vulkan::Window> ()),
              surface (std::make_unique<vulkan::Surface> (this))
    {
    }

    void Renderer::createSurface (vk::ApplicationInfo applicationInfo, int desiredWidth, int desiredHeight)
    {
        surface->create (applicationInfo, desiredWidth, desiredHeight);

    }

    vk::Device Renderer::getDevice ()
    {
        return surface->getDevice ();
    }

    vk::CommandPool Renderer::getCommandPool ()
    {
        return commandPool.get ();
    }

    vulkan::Window *Renderer::getWindow ()
    {
        return window.get ();
    }

    vulkan::Surface *Renderer::getSurface ()
    {
        return surface.get ();
    }

    bool Renderer::isEnabledValidationLayers ()
    {
        return enableValidationLayers;
    }

    std::vector<const char *> Renderer::getValidationLayers ()
    {
        return validationLayers;
    }

    std::vector<const char *> Renderer::getRequiredExtensions ()
    {
        return requiredExtensions;
    }

    uint32_t Renderer::findMemoryType (uint32_t typeFilter, vk::MemoryPropertyFlags properties)
    {
        vk::PhysicalDeviceMemoryProperties memProperties = surface->getPhysicalDevice ().getMemoryProperties ();

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

        std::vector<vk::CommandBuffer> _buffers = surface->getDevice ().allocateCommandBuffers (allocInfo);
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

        surface->getGraphicsQueue ().submit (1, &submitInfo, vk::Fence ());
        surface->getGraphicsQueue ().waitIdle ();

        surface->getDevice ().freeCommandBuffers (*commandPool, 1, &commandBuffer);
    }

}

//
// Created by tobias on 06.02.17.
//

#ifndef VULKANTEST_RENDERER_H
#define VULKANTEST_RENDERER_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "SwapChain.h"
#include "Surface.h"
#include "Window.h"

namespace vulkan
{

    class Window;

    class Surface;

    class Renderer : public helper::NonCopyable
    {
    public:
        uint32_t findMemoryType (uint32_t typeFilter, vk::MemoryPropertyFlags properties);
        vk::CommandBuffer beginSingleTimeCommands ();
        void endSingleTimeCommands (vk::CommandBuffer commandBuffer);

        vk::Device getDevice ();
        vk::CommandPool getCommandPool ();
        vulkan::Window *getWindow ();
        vulkan::Surface *getSurface ();
        bool isEnabledValidationLayers ();
        std::vector<const char *> getValidationLayers ();
        std::vector<const char *> getRequiredExtensions ();

    protected:
        Renderer (bool enableValidationLayers);

        void createSurface (vk::ApplicationInfo applicationInfo, int desiredWidth, int desiredHeight);

        bool enableValidationLayers;
        std::vector<const char *> validationLayers;
        std::vector<const char *> requiredExtensions;

        std::unique_ptr<vulkan::Window> window;
        std::unique_ptr<vulkan::Surface> surface;


        vk::UniqueCommandPool commandPool;

    };

}

#endif //VULKANTEST_RENDERER_H

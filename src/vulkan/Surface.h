//
// Created by tobias on 12.02.17.
//

#ifndef VULKANTEST_SURFACE_H
#define VULKANTEST_SURFACE_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "SwapChain.h"
#include "helper/NonCopyable.h"
#include "Renderer.h"

namespace vulkan
{

    class Renderer;

    class SwapChain;

    class Surface : public helper::NonCopyable
    {
    public:
        Surface (vulkan::Renderer *renderer);

        void create (vk::ApplicationInfo applicationInfo, int desiredWidth, int desiredHeight);
        vk::Instance getInstance ();
        vk::SurfaceKHR getSurface ();
        vk::PhysicalDevice getPhysicalDevice ();
        vk::Device getDevice ();
        vk::RenderPass getRenderPass ();
        vulkan::SwapChain *getSwapChain ();
        vk::Extent2D getSize ();
        vk::Queue getGraphicsQueue ();
        vk::Queue getPresentQueue ();

    private:
        void _createInstance (vk::ApplicationInfo applicationInfo);
        std::vector<const char *> _getRequiredExtensions ();
        bool _checkValidationLayerSupport ();
        void _setupDebugCallback ();
        void _createSurface ();
        void _pickPhysicalDevice ();
        void _createLogicalDevice ();
        void _createRenderPass ();

        static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback (VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char *layerPrefix,
                                                              const char *msg, void *userData);

        vulkan::Renderer *renderer;

        vk::UniqueInstance instance;
        vk::UniqueDebugReportCallbackEXT callback;
        vk::SurfaceKHR surface;
        vk::PhysicalDevice physicalDevice;
        vk::UniqueDevice device;
        vk::UniqueRenderPass renderPass;
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        std::unique_ptr<vulkan::SwapChain> swapChain;
    };


}

#endif //VULKANTEST_SURFACE_H

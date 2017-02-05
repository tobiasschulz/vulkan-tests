//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_SWAPCHAINSUPPORTHELPER_H
#define VULKANTEST_SWAPCHAINSUPPORTHELPER_H

#include "../helper/vulkan.h"

namespace helper
{

    class SwapChainSupport
    {
    public:
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat ();
        vk::PresentModeKHR chooseSwapPresentMode ();
        vk::Extent2D chooseSwapExtent (uint32_t width, uint32_t height);
        uint32_t getImageCount ();

        static SwapChainSupport querySwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice device);

        static bool isDeviceSuitable (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, std::vector<const char *> requiredExtensions);
        static bool doesDeviceSupportAllExtensions (vk::PhysicalDevice physicalDevice, std::vector<const char *> requiredExtensions);


    private:
        SwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice);

    };

}

#endif //VULKANTEST_SWAPCHAINSUPPORTHELPER_H

//
// Created by tobias on 05.02.17.
//

#include "SwapChainSupport.h"
#include <set>
#include "QueueFamilies.h"

namespace helper
{

    SwapChainSupport SwapChainSupport::querySwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice)
    {
        return SwapChainSupport (surface, physicalDevice);
    }

    SwapChainSupport::SwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice)
    {
        capabilities = physicalDevice.getSurfaceCapabilitiesKHR (surface);
        formats = physicalDevice.getSurfaceFormatsKHR (surface);
        presentModes = physicalDevice.getSurfacePresentModesKHR (surface);
    }

    vk::SurfaceFormatKHR SwapChainSupport::chooseSwapSurfaceFormat ()
    {
        if (formats.size () == 1 && formats[0].format == vk::Format::eUndefined) {
            return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
        }

        for (const auto &f : formats) {
            if (f.format == vk::Format::eB8G8R8A8Unorm && f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return f;
            }
        }

        return formats[0];
    }

    vk::PresentModeKHR SwapChainSupport::chooseSwapPresentMode ()
    {
        for (const auto &pm : presentModes) {
            if (pm == vk::PresentModeKHR::eMailbox) {
                return pm;
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapChainSupport::chooseSwapExtent (uint32_t width, uint32_t height)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max ()) {
            return capabilities.currentExtent;
        }
        else {
            vk::Extent2D actualExtent = { width, height };

            actualExtent.width = std::max (capabilities.minImageExtent.width,
                                           std::min (capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max (capabilities.minImageExtent.height,
                                            std::min (capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    uint32_t SwapChainSupport::getImageCount ()
    {
        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 &&
            imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }
        return imageCount;
    }

    bool SwapChainSupport::isDeviceSuitable (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, std::vector<const char *> requiredExtensions)
    {
        helper::QueueFamilies indices = helper::QueueFamilies::findQueueFamilies (surface, physicalDevice);

        bool extensionsSupported = doesDeviceSupportAllExtensions (physicalDevice, requiredExtensions);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            auto s = helper::SwapChainSupport::querySwapChainSupport (surface, physicalDevice);
            swapChainAdequate = !s.formats.empty () && !s.presentModes.empty ();
        }

        return indices.isComplete () && extensionsSupported && swapChainAdequate;
    }

    bool SwapChainSupport::doesDeviceSupportAllExtensions (vk::PhysicalDevice physicalDevice, std::vector<const char *> requiredExtensions)
    {
        std::vector<vk::ExtensionProperties> availableExtensions = physicalDevice.enumerateDeviceExtensionProperties ();

        std::set<std::string> r (requiredExtensions.begin (), requiredExtensions.end ());
        for (const auto &extension : availableExtensions) {
            r.erase (extension.extensionName);
        }
        return r.empty ();
    }

}

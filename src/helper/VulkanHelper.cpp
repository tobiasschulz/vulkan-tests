//
// Created by tobias on 05.02.17.
//

#include "VulkanHelper.h"


static SwapChainSupportDetails VulkanHelper::querySwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice device)
{
    SwapChainSupportDetails details;

    details.capabilities = device.getSurfaceCapabilitiesKHR (surface);
    details.formats = device.getSurfaceFormatsKHR (surface);
    details.presentModes = device.getSurfacePresentModesKHR (surface);

    return details;
}


vk::SurfaceFormatKHR SwapChainSupportDetails::chooseSwapSurfaceFormat ()
{
    if (formats.size () == 1 && formats[0].format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

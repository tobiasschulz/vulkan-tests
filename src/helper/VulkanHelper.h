//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_VULKANHELPER_H
#define VULKANTEST_VULKANHELPER_H

#include "../helper/vulkan.h"

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class VulkanHelper {

public:
    static SwapChainSupportDetails querySwapChainSupport (vk::SurfaceKHR surface, vk::PhysicalDevice device);

};


#endif //VULKANTEST_VULKANHELPER_H

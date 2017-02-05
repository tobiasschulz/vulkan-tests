//
// Created by tobias on 05.02.17.
//

#include "QueueFamilies.h"

namespace helper
{
    QueueFamilies::QueueFamilies ()
    {
    }

    bool QueueFamilies::isComplete ()
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }

    void QueueFamilies::apply (vk::SwapchainCreateInfoKHR &createInfo)
    {
        if (graphicsFamily != presentFamily) {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;

            uint32_t queueFamilyIndices[] = {(uint32_t) graphicsFamily, (uint32_t) presentFamily};
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }
    }

    QueueFamilies QueueFamilies::findQueueFamilies (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice)
    {
        QueueFamilies indices;

        auto queueFamilies = physicalDevice.getQueueFamilyProperties ();

        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
            }

            vk::Bool32
                    presentSupport = (vk::Bool32)
                    false;

            physicalDevice.getSurfaceSupportKHR (i, surface, &presentSupport);

            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete ()) {
                break;
            }

            i++;
        }

        return indices;
    }

}

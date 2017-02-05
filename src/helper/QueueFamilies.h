//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_QUEUEFAMILIES_H
#define VULKANTEST_QUEUEFAMILIES_H

#include "../helper/vulkan.h"

namespace helper
{

    class QueueFamilies
    {
    public:
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool isComplete ();
        void apply (vk::SwapchainCreateInfoKHR &createInfo);

        static QueueFamilies findQueueFamilies (vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice);

    private:
        QueueFamilies ();
    };

}

#endif //VULKANTEST_QUEUEFAMILIES_H

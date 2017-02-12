//
// Created by tobias on 12.02.17.
//

#ifndef VULKANTEST_SWAPCHAIN_H
#define VULKANTEST_SWAPCHAIN_H

#include "vulkan.h"
#include "helper/NonCopyable.h"
#include "Surface.h"

namespace vulkan
{

    class Surface;

    class SwapChain : public helper::NonCopyable
    {

    public:
        SwapChain (vulkan::Surface *surface);

        void create (int desiredWidth, int desiredHeight);
        vk::Extent2D getSize ();
        vk::SwapchainKHR getSwapChain ();
        std::vector<vk::Image> getImages ();
        vk::Format getImageFormat ();
        std::vector<vk::Framebuffer> getFramebuffers ();

    private:
        void _createSwapChain (int desiredWidth, int desiredHeight);
        void _createImageViews ();
        void _createFramebuffers ();

        vulkan::Surface *surface;

        vk::Extent2D swapChainExtent;
        vk::UniqueSwapchainKHR swapChain;
        std::vector<vk::Image> swapChainImages;
        vk::Format swapChainImageFormat;
        std::vector<vk::UniqueImageView> swapChainImageViews;
        std::vector<vk::UniqueFramebuffer> swapChainFramebuffers;

    };

}

#endif //VULKANTEST_SWAPCHAIN_H

//
// Created by tobias on 12.02.17.
//

#include "SwapChain.h"
#include "helper/SwapChainSupport.h"
#include "helper/QueueFamilies.h"

namespace vulkan
{
    SwapChain::SwapChain (vulkan::Surface *_surface)
            : surface (_surface)
    {
    }

    void SwapChain::create (int desiredWidth, int desiredHeight)
    {
        _createSwapChain (desiredWidth, desiredHeight);
        _createImageViews ();
        _createFramebuffers ();
    }

    vk::Extent2D SwapChain::getSize ()
    {
        return swapChainExtent;
    }

    vk::SwapchainKHR SwapChain::getSwapChain ()
    {
        return *swapChain;
    }

    std::vector<vk::Image> SwapChain::getImages ()
    {
        return swapChainImages;
    }

    vk::Format SwapChain::getImageFormat ()
    {
        return swapChainImageFormat;
    }

    std::vector<vk::Framebuffer> SwapChain::getFramebuffers ()
    {
        std::vector<vk::Framebuffer> result;
        for (std::size_t i = 0; i < swapChainFramebuffers.size (); i++) {
            result.push_back (*swapChainFramebuffers[i]);
        }
        return result;
    }

    void SwapChain::_createSwapChain (int desiredWidth, int desiredHeight)
    {
        auto device = surface->getDevice ();

        helper::SwapChainSupport s = helper::SwapChainSupport::querySwapChainSupport (surface->getSurface (), surface->getPhysicalDevice ());
        vk::SurfaceFormatKHR surfaceFormat = s.chooseSwapSurfaceFormat ();
        vk::PresentModeKHR presentMode = s.chooseSwapPresentMode ();
        vk::Extent2D extent = s.chooseSwapExtent (desiredWidth, desiredHeight);

        vk::SwapchainCreateInfoKHR createInfo ({}, surface->getSurface ());

        createInfo.minImageCount = s.getImageCount ();
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        helper::QueueFamilies families = helper::QueueFamilies::findQueueFamilies (surface->getSurface (), surface->getPhysicalDevice ());
        families.apply (createInfo);

        createInfo.preTransform = s.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        swapChain = device.createSwapchainKHRUnique (createInfo);

        swapChainImages = device.getSwapchainImagesKHR (*swapChain);

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void SwapChain::_createImageViews ()
    {
        auto device = surface->getDevice ();

        swapChainImageViews.clear ();

        for (uint32_t i = 0; i < swapChainImages.size (); i++) {
            vk::ImageViewCreateInfo createInfo;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = vk::ImageViewType::e2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = vk::ComponentSwizzle::eIdentity;
            createInfo.components.g = vk::ComponentSwizzle::eIdentity;
            createInfo.components.b = vk::ComponentSwizzle::eIdentity;
            createInfo.components.a = vk::ComponentSwizzle::eIdentity;
            createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            swapChainImageViews.push_back (device.createImageViewUnique (createInfo));
        }
    }


    void SwapChain::_createFramebuffers ()
    {
        auto device = surface->getDevice ();

        swapChainFramebuffers.clear ();

        for (size_t i = 0; i < swapChainImageViews.size (); i++) {
            vk::ImageView attachments[] = { *swapChainImageViews[i] };

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.renderPass = surface->getRenderPass ();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            swapChainFramebuffers.push_back (device.createFramebufferUnique (framebufferInfo));
        }
    }

}

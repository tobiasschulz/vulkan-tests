//
// Created by tobias on 12.02.17.
//

#include <iostream>
#include <set>
#include "Surface.h"
#include "helper/QueueFamilies.h"
#include "helper/SwapChainSupport.h"

namespace vulkan
{
    Surface::Surface (vulkan::Renderer *_renderer)
            : renderer (_renderer), swapChain (std::make_unique<vulkan::SwapChain> (this))
    {
    }

    void Surface::create (vk::ApplicationInfo applicationInfo, int desiredWidth, int desiredHeight)
    {
        _createInstance (applicationInfo);
        _setupDebugCallback ();
        _createSurface ();
        _pickPhysicalDevice ();
        _createLogicalDevice ();
        _createRenderPass ();

        swapChain->create (desiredWidth, desiredHeight);

    }

    vk::Instance Surface::getInstance ()
    {
        return *instance;
    }

    vk::SurfaceKHR Surface::getSurface ()
    {
        return surface;
    }

    vk::PhysicalDevice Surface::getPhysicalDevice ()
    {
        return physicalDevice;
    }

    vk::Device Surface::getDevice ()
    {
        return *device;
    }

    vk::RenderPass Surface::getRenderPass ()
    {
        return *renderPass;
    }

    vulkan::SwapChain *Surface::getSwapChain ()
    {
        return swapChain.get ();
    }

    vk::Extent2D Surface::getSize ()
    {
        return swapChain->getSize ();
    }

    vk::Queue Surface::getGraphicsQueue ()
    {
        return graphicsQueue;
    }

    vk::Queue Surface::getPresentQueue ()
    {
        return presentQueue;
    }

    void Surface::_createInstance (vk::ApplicationInfo applicationInfo)
    {
        if (renderer->isEnabledValidationLayers () && !_checkValidationLayerSupport ()) {
            throw std::runtime_error ("validation layers requested, but not available!");
        }

        vk::InstanceCreateInfo createInfo ({}, &applicationInfo);

        auto extensions = _getRequiredExtensions ();
        createInfo.setEnabledExtensionCount ((uint32_t) extensions.size ());
        createInfo.setPpEnabledExtensionNames (extensions.data ());

        auto validationLayers = renderer->getValidationLayers ();
        if (renderer->isEnabledValidationLayers ()) {
            std::cout << validationLayers[0] << std::endl;
            createInfo.enabledLayerCount = (uint32_t) validationLayers.size ();
            createInfo.ppEnabledLayerNames = validationLayers.data ();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        instance = vk::createInstanceUnique (createInfo);
    }

    void Surface::_setupDebugCallback ()
    {
        if (!renderer->isEnabledValidationLayers ())
            return;

        vk::DebugReportCallbackCreateInfoEXT createInfo (vk::DebugReportFlagBitsEXT::eDebug, _debugCallback);
        callback = instance->createDebugReportCallbackEXTUnique (createInfo);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Surface::_debugCallback (VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
                                                            int32_t code, const char *layerPrefix, const char *msg, void *userData)
    {
        std::cerr << "validation layer: " << msg << std::endl;

        return VK_FALSE;
    }

    std::vector<const char *> Surface::_getRequiredExtensions ()
    {
        std::vector<const char *> extensions;

        unsigned int glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);

        for (unsigned int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back (glfwExtensions[i]);
        }

        if (renderer->isEnabledValidationLayers ()) {
            extensions.push_back (VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }

    bool Surface::_checkValidationLayerSupport ()
    {
        std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties ();

        for (const char *layerName : renderer->getValidationLayers ()) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp (layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void Surface::_createSurface ()
    {
        VkSurfaceKHR result;
        if (glfwCreateWindowSurface (((VkInstance) instance.get ()), renderer->getWindow ()->getNativeWindow (), nullptr, &result) != VK_SUCCESS) {
            throw std::runtime_error ("failed to create window surface!");
        }
        surface = vk::SurfaceKHR (result);
    }

    void Surface::_pickPhysicalDevice ()
    {
        auto physicalDevices = instance->enumeratePhysicalDevices ();
        for (const auto &pd : physicalDevices) {
            if (helper::SwapChainSupport::isDeviceSuitable (surface, pd, renderer->getRequiredExtensions ())) {
                physicalDevice = pd;
                break;
            }
        }

        if (!physicalDevice) {
            throw std::runtime_error ("failed to find a suitable GPU!");
        }
    }

    void Surface::_createLogicalDevice ()
    {
        helper::QueueFamilies indices = helper::QueueFamilies::findQueueFamilies (surface, physicalDevice);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

        for (int queueFamily : uniqueQueueFamilies) {
            float queuePriority = 1.0f;
            queueCreateInfos.push_back (vk::DeviceQueueCreateInfo ({}, queueFamily, 1, &queuePriority));
        }

        vk::DeviceCreateInfo createInfo;

        createInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size ();
        createInfo.pQueueCreateInfos = queueCreateInfos.data ();

        auto requiredExtensions = renderer->getRequiredExtensions ();
        createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size ();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data ();

        auto validationLayers = renderer->getValidationLayers ();
        if (renderer->isEnabledValidationLayers ()) {
            createInfo.enabledLayerCount = (uint32_t) validationLayers.size ();
            createInfo.ppEnabledLayerNames = validationLayers.data ();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        device = physicalDevice.createDeviceUnique (createInfo);

        device->getQueue ((uint32_t) indices.graphicsFamily, 0, &graphicsQueue);
        device->getQueue ((uint32_t) indices.presentFamily, 0, &presentQueue);
    }

    void Surface::_createRenderPass ()
    {
        helper::SwapChainSupport s = helper::SwapChainSupport::querySwapChainSupport (surface, physicalDevice);
        vk::SurfaceFormatKHR surfaceFormat = s.chooseSwapSurfaceFormat ();


        vk::AttachmentDescription colorAttachment;
        colorAttachment.format =  surfaceFormat.format;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachmentRef;
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        vk::SubpassDependency dependency;
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = (vk::AccessFlagBits) 0;
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstAccessMask = vk::AccessFlags (vk::AccessFlagBits::eColorAttachmentRead) | vk::AccessFlagBits::eColorAttachmentWrite;

        vk::RenderPassCreateInfo renderPassInfo;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        renderPass = device->createRenderPassUnique (renderPassInfo);
    }

}

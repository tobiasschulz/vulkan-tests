#include "main.h"

#include "../helper/vulkan.h"
#include "../helper/io.h"
#include "../helper/helpers.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char *> VALIDATION_LAYERS = { "VK_LAYER_LUNARG_standard_validation", };

const std::vector<const char *> REQUIRED_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };

#ifdef NDEBUG
bool enableValidationLayers = false;
#else
bool enableValidationLayers = true;
#endif

class HelloTriangleApplication : public helper::Renderer
{
public:
    HelloTriangleApplication () : uniformBuffer (*this)
    {

    }

    void run ()
    {
        initWindow ();
        initVulkan ();
        mainLoop ();
    }

private:
    GLFWwindow *window;

    vk::UniqueDebugReportCallbackEXT callback;
    vk::SurfaceKHR surface;

    vk::UniqueSwapchainKHR swapChain;

    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat;
    std::vector<vk::UniqueImageView> swapChainImageViews;
    std::vector<vk::UniqueFramebuffer> swapChainFramebuffers;

    vk::UniqueRenderPass renderPass;
    vk::UniqueDescriptorSetLayout descriptorSetLayout;
    vk::UniquePipelineLayout pipelineLayout;
    vk::UniquePipeline graphicsPipeline;

    std::vector<vk::UniqueCommandBuffer> commandBuffers;

    std::vector<std::shared_ptr<helper::Texture>> textures;
    std::vector<std::shared_ptr<helper::Mesh>> meshes;
    helper::UniformBuffer uniformBuffer;

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSet descriptorSet;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;

    vk::UniqueInstance instance;

    void initWindow ()
    {
        glfwInit ();

        glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow (WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan ()
    {
        createInstance ();
        setupDebugCallback ();
        createSurface ();
        pickPhysicalDevice ();
        createLogicalDevice ();
        createSwapChain ();
        createImageViews ();
        createRenderPass ();
        createDescriptorSetLayout ();
        createGraphicsPipeline ();
        createFramebuffers ();
        createCommandPool ();
        createTextures ();
        createMeshes ();
        createUniformBuffer ();
        createDescriptorPool ();
        createDescriptorSet ();
        createCommandBuffers ();
        createSemaphores ();
    }

    void mainLoop ()
    {
        while (!glfwWindowShouldClose (window)) {
            glfwPollEvents ();
            uniformBuffer.update ();
            drawFrame ();
        }

        device->waitIdle ();

        glfwDestroyWindow (window);
    }

    void createInstance ()
    {
        if (enableValidationLayers && !checkValidationLayerSupport ()) {
            throw std::runtime_error ("validation layers requested, but not available!");
        }

        vk::ApplicationInfo appInfo (
                "Hello Triangle 2",
                VK_MAKE_VERSION(1, 0, 0),
                "No Engine",
                VK_MAKE_VERSION(1, 0, 0),
                VK_API_VERSION_1_0
        );
        vk::InstanceCreateInfo createInfo ({}, &appInfo);

        auto extensions = getRequiredExtensions ();
        createInfo.setEnabledExtensionCount ((uint32_t) extensions.size ());
        createInfo.setPpEnabledExtensionNames (extensions.data ());

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = (uint32_t) VALIDATION_LAYERS.size ();
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data ();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        instance = vk::createInstanceUnique (createInfo);
    }

    void setupDebugCallback ()
    {
        if (!enableValidationLayers)
            return;

        vk::DebugReportCallbackCreateInfoEXT createInfo (vk::DebugReportFlagBitsEXT::eDebug, debugCallback);
        callback = instance->createDebugReportCallbackEXTUnique (createInfo);
    }

    void createSurface ()
    {
        VkSurfaceKHR result;
        if (glfwCreateWindowSurface (((VkInstance) instance.get ()), window, nullptr, &result) != VK_SUCCESS) {
            throw std::runtime_error ("failed to create window surface!");
        }
        surface = vk::SurfaceKHR (result);
    }

    void pickPhysicalDevice ()
    {
        auto physicalDevices = instance->enumeratePhysicalDevices ();
        for (const auto &pd : physicalDevices) {
            if (helper::SwapChainSupport::isDeviceSuitable (surface, pd, REQUIRED_EXTENSIONS)) {
                physicalDevice = pd;
                break;
            }
        }

        if (!physicalDevice) {
            throw std::runtime_error ("failed to find a suitable GPU!");
        }
    }

    void createLogicalDevice ()
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

        createInfo.enabledExtensionCount = (uint32_t) REQUIRED_EXTENSIONS.size ();
        createInfo.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data ();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = (uint32_t) VALIDATION_LAYERS.size ();
            createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data ();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        device = physicalDevice.createDeviceUnique (createInfo);

        device->getQueue ((uint32_t) indices.graphicsFamily, 0, &graphicsQueue);
        device->getQueue ((uint32_t) indices.presentFamily, 0, &presentQueue);
    }

    void createSwapChain ()
    {
        helper::SwapChainSupport s = helper::SwapChainSupport::querySwapChainSupport (surface, physicalDevice);

        vk::SurfaceFormatKHR surfaceFormat = s.chooseSwapSurfaceFormat ();
        vk::PresentModeKHR presentMode = s.chooseSwapPresentMode ();
        vk::Extent2D extent = s.chooseSwapExtent (WIDTH, HEIGHT);

        vk::SwapchainCreateInfoKHR createInfo ({}, surface);

        createInfo.minImageCount = s.getImageCount ();
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        helper::QueueFamilies families = helper::QueueFamilies::findQueueFamilies (surface, physicalDevice);
        families.apply (createInfo);

        createInfo.preTransform = s.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        swapChain = device->createSwapchainKHRUnique (createInfo);

        swapChainImages = device->getSwapchainImagesKHR (*swapChain);

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }

    void createImageViews ()
    {
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

            swapChainImageViews.push_back (device->createImageViewUnique (createInfo));
        }
    }

    void createRenderPass ()
    {
        vk::AttachmentDescription colorAttachment;
        colorAttachment.format = swapChainImageFormat;
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

    void createDescriptorSetLayout ()
    {
        vk::DescriptorSetLayoutBinding uboLayoutBinding;
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

        vk::DescriptorSetLayoutBinding samplerLayoutBinding;
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

        std::array<vk::DescriptorSetLayoutBinding, 2> bindings ({{ uboLayoutBinding, samplerLayoutBinding }});

        vk::DescriptorSetLayoutCreateInfo layoutInfo ({}, (uint32_t) bindings.size (), bindings.data ());

        descriptorSetLayout = device->createDescriptorSetLayoutUnique (layoutInfo);
    }

    void createGraphicsPipeline ()
    {
        helper::Shader vertShader = helper::Shader::loadShader (*device, vk::ShaderStageFlagBits::eVertex, "shader3.vert.spv");
        helper::Shader fragShader = helper::Shader::loadShader (*device, vk::ShaderStageFlagBits::eFragment, "shader3.frag.spv");

        vk::PipelineShaderStageCreateInfo shaderStages[] = {
                vertShader.getPipelineShaderStageCreateInfo ("main"),
                fragShader.getPipelineShaderStageCreateInfo ("main"),
        };

        auto bindingDescription = helper::Vertex::getBindingDescription ();
        auto attributeDescriptions = helper::Vertex::getAttributeDescriptions ();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t) attributeDescriptions.size ();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data ();

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        vk::Viewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vk::Rect2D scissor (vk::Offset2D (0, 0), swapChainExtent);

        vk::PipelineViewportStateCreateInfo viewportState;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        vk::PipelineRasterizationStateCreateInfo rasterizer;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise; ////////////////
        rasterizer.depthBiasEnable = VK_FALSE;

        vk::PipelineMultisampleStateCreateInfo multisampling;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask =
                vk::ColorComponentFlags (vk::ColorComponentFlagBits::eR) | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;

        vk::PipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        vk::DescriptorSetLayout setLayouts[] = { *descriptorSetLayout };
        pipelineLayoutInfo.pSetLayouts = setLayouts;

        pipelineLayout = device->createPipelineLayoutUnique (pipelineLayoutInfo);


        vk::GraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = *pipelineLayout;
        pipelineInfo.renderPass = *renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        vk::PipelineCache dummyCache; // = device->createPipelineCache (vk::PipelineCacheCreateInfo (vk::PipelineCacheCreateFlags ()));

        graphicsPipeline = device->createGraphicsPipelineUnique (dummyCache, pipelineInfo);

        /*VkGraphicsPipelineCreateInfo i = (VkGraphicsPipelineCreateInfo) pipelineInfo;
        VkPipeline p;
        if (vkCreateGraphicsPipelines ((VkDevice) device.get (), VK_NULL_HANDLE, 1, &i, nullptr, &p) !=
            VK_SUCCESS) {
            throw std::runtime_error ("failed to create graphics pipeline!");
        }
        graphicsPipeline = vk::UniquePipeline (vk::Pipeline (p));*/
    }

    void createFramebuffers ()
    {
        swapChainFramebuffers.clear ();

        for (size_t i = 0; i < swapChainImageViews.size (); i++) {
            vk::ImageView attachments[] = { *swapChainImageViews[i] };

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.renderPass = *renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            swapChainFramebuffers.push_back (device->createFramebufferUnique (framebufferInfo));
        }
    }

    void createCommandPool ()
    {
        helper::QueueFamilies queueFamilies = helper::QueueFamilies::findQueueFamilies (surface, physicalDevice);
        commandPool = device->createCommandPoolUnique (vk::CommandPoolCreateInfo ({}, queueFamilies.graphicsFamily));
    }

    void createTextures ()
    {
        std::shared_ptr<helper::Texture> tex1 = std::make_shared<helper::Texture> (*this, "test.png");
        textures.push_back (tex1);
    }

    void createMeshes ()
    {

        std::shared_ptr<helper::Mesh> mesh1 = std::make_shared<helper::Mesh> (*this);
        meshes.push_back (mesh1);
    }


    void createUniformBuffer ()
    {
        uniformBuffer.create ();
    }

    void createDescriptorPool ()
    {
        std::array<vk::DescriptorPoolSize, 2> poolSizes;
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = 1;
        poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
        poolSizes[1].descriptorCount = 1;

        vk::DescriptorPoolCreateInfo poolInfo;
        poolInfo.poolSizeCount = (uint32_t) poolSizes.size ();
        poolInfo.pPoolSizes = poolSizes.data ();
        poolInfo.maxSets = 1;

        descriptorPool = device->createDescriptorPoolUnique (poolInfo);
    }

    void createDescriptorSet ()
    {
        vk::DescriptorSetLayout layouts[] = { *descriptorSetLayout };

        vk::DescriptorSetAllocateInfo allocInfo = {};
        allocInfo.descriptorPool = *descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts;

        std::vector<vk::UniqueDescriptorSet> _descriptorSets = device->allocateDescriptorSetsUnique (allocInfo); ////
        descriptorSet = std::move (_descriptorSets.front ());

        // descriptor writes
        std::vector<vk::WriteDescriptorSet> descriptorWrites;

        // write uniform buffer object
        vk::DescriptorBufferInfo uboInfo (*uniformBuffer.uniformBuffer, 0, sizeof (helper::UniformBufferObject));
        vk::WriteDescriptorSet uboWrite;
        uboWrite.dstSet = *descriptorSet;
        uboWrite.dstBinding = 0;
        uboWrite.dstArrayElement = 0;
        uboWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
        uboWrite.descriptorCount = 1;
        uboWrite.pBufferInfo = &uboInfo;
        descriptorWrites.push_back (uboWrite);

        // write textures
        for (uint32_t t = 0; t < textures.size (); t++) {
            auto &texture = textures[t];

            vk::DescriptorImageInfo imageInfo (*texture->textureSampler, *texture->textureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);
            vk::WriteDescriptorSet texWrite;
            texWrite.dstSet = *descriptorSet;
            texWrite.dstBinding = 1;
            texWrite.dstArrayElement = 0;
            texWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
            texWrite.descriptorCount = 1;
            texWrite.pImageInfo = &imageInfo;
            descriptorWrites.push_back (texWrite);
        }

        std::cout << "descriptorWrites: " << descriptorWrites.size () << std::endl;

        device->updateDescriptorSets (descriptorWrites, nullptr);
    }

    void createCommandBuffers ()
    {
        vk::CommandBufferAllocateInfo allocInfo (
                *commandPool,
                vk::CommandBufferLevel::ePrimary,
                (uint32_t) swapChainFramebuffers.size ()
        );
        commandBuffers = device->allocateCommandBuffersUnique (allocInfo);

        for (size_t i = 0; i < commandBuffers.size (); i++) {
            auto &cmd = commandBuffers[i];

            vk::CommandBufferBeginInfo beginInfo (vk::CommandBufferUsageFlagBits::eSimultaneousUse);


            vk::RenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = *renderPass;
            renderPassInfo.framebuffer = *swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = vk::Offset2D (0, 0);
            renderPassInfo.renderArea.extent = swapChainExtent;

            vk::ClearValue clearColor = vk::ClearColorValue (std::array<float, 4>{{ 0.f, 0.f, 0.f, 1.f }});
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            cmd->begin (beginInfo);
            cmd->beginRenderPass (renderPassInfo, vk::SubpassContents::eInline);
            cmd->bindPipeline (vk::PipelineBindPoint::eGraphics, *graphicsPipeline);

            for (uint32_t m = 0; m < meshes.size (); m++) {
                auto &mesh = meshes[m];

                cmd->bindVertexBuffers (0, std::array<vk::Buffer, 1>{ mesh->getVertexBuffer () }, std::array<vk::DeviceSize, 1>{ 0 });
                cmd->bindIndexBuffer (mesh->getIndexBuffer (), 0, vk::IndexType::eUint16);

                cmd->bindDescriptorSets (
                        vk::PipelineBindPoint::eGraphics,
                        *pipelineLayout,
                        0,
                        std::array<vk::DescriptorSet, 1> { *descriptorSet },
                        std::array<uint32_t, 0> { }
                );

                cmd->drawIndexed (mesh->getIndexCount (), 1, 0, 0, 0);
            }

            cmd->endRenderPass ();
            cmd->end ();
        }
    }

    void createSemaphores ()
    {
        imageAvailableSemaphore = device->createSemaphore (vk::SemaphoreCreateInfo ());
        renderFinishedSemaphore = device->createSemaphore (vk::SemaphoreCreateInfo ());
    }

    void drawFrame ()
    {
        uint32_t imageIndex;

        device->acquireNextImageKHR (
                *swapChain,
                std::numeric_limits<uint64_t>::max (), // timeout = max 64 bit value
                imageAvailableSemaphore,
                vk::Fence (), // empty fence (no clue what a fence is)
                &imageIndex
        );

        vk::SubmitInfo submitInfo;

        submitInfo.waitSemaphoreCount = 1;
        const vk::Semaphore pWaitSemaphores[] = { imageAvailableSemaphore };
        submitInfo.pWaitSemaphores = pWaitSemaphores;
        const vk::PipelineStageFlags pWaitDstStageMask[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        submitInfo.pWaitDstStageMask = pWaitDstStageMask;

        const vk::CommandBuffer pCommandBuffers[] = { *commandBuffers[imageIndex] };
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = pCommandBuffers;

        vk::Semaphore signalSemaphores[] = { renderFinishedSemaphore };

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        graphicsQueue.submit (
                std::vector<vk::SubmitInfo> ({ submitInfo }),
                vk::Fence () // empty fence
        );

        vk::PresentInfoKHR presentInfo;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        const vk::SwapchainKHR pSwapchains[] = { *swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = pSwapchains;

        presentInfo.pImageIndices = &imageIndex;

        presentQueue.presentKHR (presentInfo);
    }

    std::vector<const char *> getRequiredExtensions ()
    {
        std::vector<const char *> extensions;

        unsigned int glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);

        for (unsigned int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back (glfwExtensions[i]);
        }

        if (enableValidationLayers) {
            extensions.push_back (VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }

    bool checkValidationLayerSupport ()
    {
        std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties ();

        for (const char *layerName : VALIDATION_LAYERS) {
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

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback (VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
                   int32_t code, const char *layerPrefix, const char *msg, void *userData)
    {
        std::cerr << "validation layer: " << msg << std::endl;

        return VK_FALSE;
    }
};

int main ()
{
    HelloTriangleApplication app;

    try {
        app.run ();
    }
    catch (const std::runtime_error &e) {
        std::cerr << e.what () << std::endl;
        exit (0);
        return EXIT_FAILURE;
    }
    exit (0);

    return EXIT_SUCCESS;
}

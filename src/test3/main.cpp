#include "main.h"

#include "../vulkan/vulkan.h"
#include "../vulkan/helper/io.h"
#include "../vulkan/helper/helpers.h"
#include "../vulkan/effects/RotatingMesh.h"

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

#ifdef NDEBUG
bool ENABLE_VALIDATION_LAYERS = false;
#else
bool ENABLE_VALIDATION_LAYERS = true;
#endif

class HelloTriangleApplication : public vulkan::Renderer
{
public:
    HelloTriangleApplication ()
            : Renderer (ENABLE_VALIDATION_LAYERS),
              uniformBuffer (this)
    {
    }

    void run ()
    {
        initWindow ();
        initVulkan ();
        mainLoop ();
    }

private:
    vk::UniqueDescriptorSetLayout descriptorSetLayout;
    vk::UniquePipelineLayout pipelineLayout;
    vk::UniquePipeline graphicsPipeline;

    std::vector<vk::UniqueCommandBuffer> commandBuffers;

    std::vector<std::shared_ptr<helper::Texture>> textures;
    std::vector<std::shared_ptr<vulkan::Mesh>> meshes;
    vulkan::FirstPersonCamera camera;
    vulkan::UniformBuffer<vulkan::FirstPersonCamera::UniformBufferObject> uniformBuffer;

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSet descriptorSet;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;


    void initWindow ()
    {
        window->create (WIDTH, HEIGHT);

        glfwSetWindowUserPointer (window->getNativeWindow (), this);
        glfwSetKeyCallback (window->getNativeWindow (), [] (GLFWwindow *w, int key, int scancode, int action, int mods) {
            // Here we retrieve the pointer we setted before. It will be equal to `this`
            auto &that = *static_cast<HelloTriangleApplication *>(glfwGetWindowUserPointer (w));
            that.handleKeypress (w, key, scancode, action, mods);
        });
    }

    void initVulkan ()
    {
        vk::ApplicationInfo appInfo (
                "Hello Triangle 3",
                VK_MAKE_VERSION(1, 0, 0),
                "No Engine",
                VK_MAKE_VERSION(1, 0, 0),
                VK_API_VERSION_1_0
        );

        createSurface (appInfo, WIDTH, HEIGHT);
        createDescriptorSetLayout ();
        createGraphicsPipeline ();
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
        while (!glfwWindowShouldClose (window->getNativeWindow ())) {
            glfwPollEvents ();

            camera.update (this);
            uniformBuffer.update (camera.getUniformBufferObject ());
            for (uint32_t m = 0; m < meshes.size (); m++) {
                auto &mesh = meshes[m];
                mesh->update (&camera);
            }
            createCommandBuffers ();

            drawFrame ();
        }

        surface->getDevice ().waitIdle ();

        glfwDestroyWindow (window->getNativeWindow ());
    }

    void handleKeypress (GLFWwindow *w, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose (w, GL_TRUE);
        }
        else {
            camera.handleKeypress (key, action, mods);
        }
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

        descriptorSetLayout = surface->getDevice ().createDescriptorSetLayoutUnique (layoutInfo);
    }

    void createGraphicsPipeline ()
    {
        helper::Shader vertShader = helper::Shader::loadShader (surface->getDevice (), vk::ShaderStageFlagBits::eVertex, "shader3.vert.spv");
        helper::Shader fragShader = helper::Shader::loadShader (surface->getDevice (), vk::ShaderStageFlagBits::eFragment, "shader3.frag.spv");

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
        viewport.width = (float) surface->getSize ().width;
        viewport.height = (float) surface->getSize ().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vk::Rect2D scissor (vk::Offset2D (0, 0), surface->getSize ());

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
        rasterizer.frontFace = vk::FrontFace::eCounterClockwise; ////////////////
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

        pipelineLayout = surface->getDevice ().createPipelineLayoutUnique (pipelineLayoutInfo);


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
        pipelineInfo.renderPass = surface->getRenderPass ();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        vk::PipelineCache dummyCache; // = device->createPipelineCache (vk::PipelineCacheCreateInfo (vk::PipelineCacheCreateFlags ()));

        graphicsPipeline = surface->getDevice ().createGraphicsPipelineUnique (dummyCache, pipelineInfo);

        /*VkGraphicsPipelineCreateInfo i = (VkGraphicsPipelineCreateInfo) pipelineInfo;
        VkPipeline p;
        if (vkCreateGraphicsPipelines ((VkDevice) device.get (), VK_NULL_HANDLE, 1, &i, nullptr, &p) !=
            VK_SUCCESS) {
            throw std::runtime_error ("failed to create graphics pipeline!");
        }
        graphicsPipeline = vk::UniquePipeline (vk::Pipeline (p));*/
    }

    void createCommandPool ()
    {
        helper::QueueFamilies queueFamilies = helper::QueueFamilies::findQueueFamilies (surface->getSurface (), surface->getPhysicalDevice ());
        commandPool = surface->getDevice ().createCommandPoolUnique (vk::CommandPoolCreateInfo ({}, queueFamilies.graphicsFamily));
    }

    void createTextures ()
    {
        std::shared_ptr<helper::Texture> tex1 = std::make_shared<helper::Texture> (*this, "test.png");
        textures.push_back (tex1);
    }

    void createMeshes ()
    {
        const std::vector<helper::Vertex> vertices_1 = {
                {{ -1.0f, 5.1f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
                {{ 1.0f,  5.1f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
                {{ 1.0f,  5.1f, 1.0f },  { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
                {{ -1.0f, 5.1f, 1.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
        };

        const std::vector<helper::Vertex> vertices_4 = {
                {{ 1.0f, 3.1f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
                {{ 3.0f, 3.1f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
                {{ 3.0f, 3.1f, 1.0f },  { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
                {{ 1.0f, 3.1f, 1.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
        };

        const std::vector<helper::Vertex> vertices_2 = {
                {{ -10.0f, -1.0f, -10.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
                {{ 10.0f,  -1.0f, -10.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
                {{ 10.0f,  -1.0f, 10.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
                {{ -10.0f, -1.0f, 10.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
        };

        const std::vector<uint16_t> indices = {
                   0, 1, 2, 2, 3, 0,
                   2, 1, 0, 0, 3, 2,
                //0, 3, 2,
        };


        std::shared_ptr<vulkan::Mesh> mesh1 = std::make_shared<effects::RotatingMesh> (this);
        mesh1->add (vertices_1, indices);
        mesh1->create ();
        meshes.push_back (mesh1);

        std::shared_ptr<vulkan::Mesh> mesh2 = std::make_shared<vulkan::Mesh> (this);
        mesh2->add (vertices_2, indices);
        mesh2->create ();
        meshes.push_back (mesh2);
    }

    void createUniformBuffer ()
    {
        uniformBuffer.create (camera.getUniformBufferObject ());
    }

    void createDescriptorPool ()
    {
        auto device = surface->getDevice ();

        std::array<vk::DescriptorPoolSize, 2> poolSizes;
        poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
        poolSizes[0].descriptorCount = 1;
        poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
        poolSizes[1].descriptorCount = 1;

        vk::DescriptorPoolCreateInfo poolInfo;
        poolInfo.poolSizeCount = (uint32_t) poolSizes.size ();
        poolInfo.pPoolSizes = poolSizes.data ();
        poolInfo.maxSets = 1;

        descriptorPool = device.createDescriptorPoolUnique (poolInfo);
    }

    void createDescriptorSet ()
    {
        auto device = surface->getDevice ();

        vk::DescriptorSetLayout layouts[] = { *descriptorSetLayout };

        vk::DescriptorSetAllocateInfo allocInfo = {};
        allocInfo.descriptorPool = *descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts;

        std::vector<vk::UniqueDescriptorSet> _descriptorSets = device.allocateDescriptorSetsUnique (allocInfo); ////
        descriptorSet = std::move (_descriptorSets.front ());

        // descriptor writes
        std::vector<vk::WriteDescriptorSet> descriptorWrites;

        // write uniform buffer object
        vk::DescriptorBufferInfo uboInfo (uniformBuffer.getBuffer (), 0, uniformBuffer.getBufferSize ());
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

        device.updateDescriptorSets (descriptorWrites, nullptr);
    }

    void createCommandBuffers ()
    {
        auto device = surface->getDevice ();
        auto framebuffers = surface->getSwapChain ()->getFramebuffers ();
        auto windowSize = surface->getSize ();

        if (commandBuffers.size () == 0) {
            vk::CommandBufferAllocateInfo allocInfo (
                    *commandPool,
                    vk::CommandBufferLevel::ePrimary,
                    (uint32_t) framebuffers.size ()
            );
            commandBuffers = device.allocateCommandBuffersUnique (allocInfo);
        }

        for (size_t i = 0; i < commandBuffers.size (); i++) {
            auto &cmd = commandBuffers[i];

            vk::CommandBufferBeginInfo beginInfo (vk::CommandBufferUsageFlagBits::eSimultaneousUse);


            vk::RenderPassBeginInfo renderPassInfo;
            auto p = surface->getRenderPass ();
            renderPassInfo.renderPass = p;
            renderPassInfo.framebuffer = framebuffers[i];
            renderPassInfo.renderArea.offset = vk::Offset2D (0, 0);
            renderPassInfo.renderArea.extent = windowSize;

            vk::ClearValue clearColor = vk::ClearColorValue (std::array<float, 4>{{ 0.f, 0.f, 0.f, 1.f }});
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            cmd->begin (beginInfo);
            cmd->beginRenderPass (renderPassInfo, vk::SubpassContents::eInline);
            cmd->bindPipeline (vk::PipelineBindPoint::eGraphics, *graphicsPipeline);

            for (uint32_t m = 0; m < meshes.size (); m++) {
                auto &mesh = meshes[m];

                cmd->bindVertexBuffers (0, std::array<vk::Buffer, 1>{{ mesh->getVertexBuffer () }}, std::array<vk::DeviceSize, 1>{{ 0 }});
                cmd->bindIndexBuffer (mesh->getIndexBuffer (), 0, vk::IndexType::eUint16);

                cmd->bindDescriptorSets (
                        vk::PipelineBindPoint::eGraphics,
                        *pipelineLayout,
                        0,
                        std::array<vk::DescriptorSet, 1> {{ *descriptorSet }},
                        std::array<uint32_t, 0> {}
                );

                cmd->drawIndexed (mesh->getIndexCount (), 1, 0, 0, 0);
            }

            cmd->endRenderPass ();
            cmd->end ();
        }
    }

    void createSemaphores ()
    {
        imageAvailableSemaphore = surface->getDevice ().createSemaphore (vk::SemaphoreCreateInfo ());
        renderFinishedSemaphore = surface->getDevice ().createSemaphore (vk::SemaphoreCreateInfo ());
    }

    void drawFrame ()
    {
        auto device = surface->getDevice ();

        uint32_t imageIndex;

        device.acquireNextImageKHR (
                surface->getSwapChain ()->getSwapChain (),
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

        surface->getGraphicsQueue ().submit (
                std::vector<vk::SubmitInfo> ({ submitInfo }),
                vk::Fence () // empty fence
        );

        vk::PresentInfoKHR presentInfo;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        const vk::SwapchainKHR pSwapchains[] = { surface->getSwapChain ()->getSwapChain () };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = pSwapchains;

        presentInfo.pImageIndices = &imageIndex;

        surface->getPresentQueue ().presentKHR (presentInfo);
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

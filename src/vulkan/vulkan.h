
#define vkCreateDebugReportCallbackEXT(a, b, c, d) __vkCreateDebugReportCallbackEXT(a, b, c, d)
#define vkDestroyDebugReportCallbackEXT(a, b, c) __vkDestroyDebugReportCallbackEXT(a, b, c)

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "../../3rdparty/stb/stb_image.h"

namespace vk
{
    typedef std::shared_ptr<UniqueBuffer> SharedBuffer;
    typedef std::shared_ptr<UniqueBufferView> SharedBufferView;
    typedef std::shared_ptr<UniqueCommandBuffer> SharedCommandBuffer;
    typedef std::shared_ptr<UniqueCommandPool> SharedCommandPool;
    typedef std::shared_ptr<UniqueDebugReportCallbackEXT> SharedDebugReportCallbackEXT;
    typedef std::shared_ptr<UniqueDescriptorPool> SharedDescriptorPool;
    typedef std::shared_ptr<UniqueDescriptorSetLayout> SharedDescriptorSetLayout;
    typedef std::shared_ptr<UniqueDescriptorSet> SharedDescriptorSet;
    typedef std::shared_ptr<UniqueDeviceMemory> SharedDeviceMemory;
    typedef std::shared_ptr<UniqueDevice> SharedDevice;
    typedef std::shared_ptr<UniqueEvent> SharedEvent;
    typedef std::shared_ptr<UniqueFence> SharedFence;
    typedef std::shared_ptr<UniqueFramebuffer> SharedFramebuffer;
    typedef std::shared_ptr<UniqueImage> SharedImage;
    typedef std::shared_ptr<UniqueImageView> SharedImageView;
    typedef std::shared_ptr<UniqueIndirectCommandsLayoutNVX> SharedIndirectCommandsLayoutNVX;
    typedef std::shared_ptr<UniqueInstance> SharedInstance;
    typedef std::shared_ptr<UniqueObjectTableNVX> SharedObjectTableNVX;
    typedef std::shared_ptr<UniquePipelineCache> SharedPipelineCache;
    typedef std::shared_ptr<UniquePipelineLayout> SharedPipelineLayout;
    typedef std::shared_ptr<UniquePipeline> SharedPipeline;
    typedef std::shared_ptr<UniqueQueryPool> SharedQueryPool;
    typedef std::shared_ptr<UniqueRenderPass> SharedRenderPass;
    typedef std::shared_ptr<UniqueSampler> SharedSampler;
    typedef std::shared_ptr<UniqueSemaphore> SharedSemaphore;
    typedef std::shared_ptr<UniqueShaderModule> SharedShaderModule;
    typedef std::shared_ptr<UniqueSurfaceKHR> SharedSurfaceKHR;
    typedef std::shared_ptr<UniqueSwapchainKHR> SharedSwapchainKHR;
}


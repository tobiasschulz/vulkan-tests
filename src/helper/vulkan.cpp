//
// Created by tobias on 05.02.17.
//

#include "vulkan.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../../3rdparty/stb/stb_image.h"

VkResult __vkCreateDebugReportCallbackEXT (VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                           const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr (instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func (instance, pCreateInfo, pAllocator, pCallback);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void __vkDestroyDebugReportCallbackEXT (VkInstance instance, VkDebugReportCallbackEXT callback,
                                        const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr (instance,
                                                                             "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func (instance, callback, pAllocator);
    }
}

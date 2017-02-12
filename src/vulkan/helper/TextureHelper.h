//
// Created by tobias on 06.02.17.
//

#ifndef VULKANTEST_TEXTUREHELPER_H
#define VULKANTEST_TEXTUREHELPER_H

#include "../vulkan.h"
#include "../Renderer.h"

namespace helper
{

    class TextureHelper
    {
    public:

        static std::pair<vk::Image, vk::DeviceMemory>
        createImage (vulkan::Renderer &renderer, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties);

        static void transitionImageLayout (vulkan::Renderer &renderer, vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
        static void copyImage (vulkan::Renderer &renderer, vk::Image srcImage, vk::Image dstImage, uint32_t width, uint32_t height);

        static vk::UniqueImageView createImageView (vk::Device device, vk::Image image, vk::Format format);

    };

}

#endif //VULKANTEST_TEXTUREHELPER_H

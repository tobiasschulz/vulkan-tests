//
// Created by tobias on 06.02.17.
//

#include "TextureHelper.h"

namespace helper
{


    std::pair<vk::Image, vk::DeviceMemory>
    TextureHelper::createImage (helper::Renderer &renderer, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::Device device = renderer.getDevice ();

        vk::ImageCreateInfo imageInfo;
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = vk::ImageLayout::ePreinitialized;
        imageInfo.usage = usage;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        vk::Image image = device.createImage (imageInfo);

        vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements (image);

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = renderer.findMemoryType (memRequirements.memoryTypeBits, properties);

        vk::DeviceMemory imageMemory = device.allocateMemory (allocInfo);
        device.bindImageMemory (image, imageMemory, 0);

        return std::make_pair (image, imageMemory);
    }


    void TextureHelper::transitionImageLayout (helper::Renderer &renderer, vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
    {
        vk::CommandBuffer commandBuffer = renderer.beginSingleTimeCommands ();

        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferSrcOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
        }
        else if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferDstOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
        }
        else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
        }
        else {
            throw std::invalid_argument ("unsupported layout transition!");
        }

        commandBuffer.pipelineBarrier (
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::DependencyFlags (),
                nullptr,
                nullptr,
                std::array<vk::ImageMemoryBarrier, 1> {{ barrier }}
        );

        renderer.endSingleTimeCommands (commandBuffer);
    }


    void TextureHelper::copyImage (helper::Renderer &renderer, vk::Image srcImage, vk::Image dstImage, uint32_t width, uint32_t height)
    {
        vk::CommandBuffer commandBuffer = renderer.beginSingleTimeCommands ();

        vk::ImageSubresourceLayers subResource;
        subResource.aspectMask = vk::ImageAspectFlagBits::eColor;
        subResource.baseArrayLayer = 0;
        subResource.mipLevel = 0;
        subResource.layerCount = 1;

        vk::ImageCopy region;
        region.srcSubresource = subResource;
        region.dstSubresource = subResource;
        region.srcOffset = vk::Offset3D (0, 0, 0);
        region.dstOffset = vk::Offset3D (0, 0, 0);
        region.extent.width = width;
        region.extent.height = height;
        region.extent.depth = 1;

        commandBuffer.copyImage (
                srcImage, vk::ImageLayout::eTransferSrcOptimal,
                dstImage, vk::ImageLayout::eTransferDstOptimal,
                std::array<vk::ImageCopy, 1>{{ region }}
        );

        renderer.endSingleTimeCommands (commandBuffer);

    }


    vk::UniqueImageView TextureHelper::createImageView (vk::Device device, vk::Image image, vk::Format format)
    {
        vk::ImageViewCreateInfo viewInfo;
        viewInfo.image = image;
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        return device.createImageViewUnique (viewInfo);
    }
}
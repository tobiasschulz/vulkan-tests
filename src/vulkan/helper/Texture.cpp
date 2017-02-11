//
// Created by tobias on 05.02.17.
//

#include "Texture.h"

namespace helper
{
    Texture::Texture (vulkan::Renderer &_renderer, std::string _filename)
            : filename (_filename), renderer (_renderer)
    {
        // create image

        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load (filename.c_str (), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        vk::DeviceSize imageSize = (uint64_t) texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error ("failed to load texture image: " + filename);
        }

        vk::Image stagingImage;
        vk::DeviceMemory stagingImageMemory;

        std::tie (stagingImage, stagingImageMemory) = helper::TextureHelper::createImage (
                renderer,
                texWidth, texHeight,
                vk::Format::eR8G8B8A8Unorm,
                vk::ImageTiling::eLinear,
                vk::ImageUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        vk::ImageSubresource subresource;
        subresource.mipLevel = 0;
        subresource.arrayLayer = 0;
        subresource.aspectMask = vk::ImageAspectFlagBits::eColor; /////////////////////

        vk::SubresourceLayout stagingImageLayout = renderer.getDevice ().getImageSubresourceLayout (stagingImage, subresource);


        void *data = renderer.getDevice ().mapMemory (stagingImageMemory, 0, imageSize);

        if (stagingImageLayout.rowPitch == texWidth * 4) {
            memcpy (data, pixels, (size_t) imageSize);
        }
        else {
            uint8_t *dataBytes = reinterpret_cast<uint8_t *>(data);

            for (int y = 0; y < texHeight; y++) {
                memcpy (&dataBytes[y * stagingImageLayout.rowPitch], &pixels[y * texWidth * 4], texWidth * 4);
            }
        }

        renderer.getDevice ().unmapMemory (stagingImageMemory);

        stbi_image_free (pixels);

        vk::Image _textureImage;
        vk::DeviceMemory _textureImageMemory;
        std::tie (_textureImage, _textureImageMemory) = helper::TextureHelper::createImage (
                renderer,
                texWidth,
                texHeight,
                vk::Format::eR8G8B8A8Unorm,
                vk::ImageTiling::eOptimal, vk::ImageUsageFlags (vk::ImageUsageFlagBits::eTransferDst) | vk::ImageUsageFlagBits::eSampled,
                vk::MemoryPropertyFlagBits::eDeviceLocal
        );
        textureImage = vk::UniqueImage (_textureImage);
        textureImageMemory = vk::UniqueDeviceMemory (_textureImageMemory);

        helper::TextureHelper::transitionImageLayout (renderer, stagingImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferSrcOptimal);
        helper::TextureHelper::transitionImageLayout (renderer, *textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferDstOptimal);
        helper::TextureHelper::copyImage (renderer, stagingImage, *textureImage, texWidth, texHeight);
        helper::TextureHelper::transitionImageLayout (renderer, *textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

        // create image view

        textureImageView = helper::TextureHelper::createImageView (renderer.getDevice (), *textureImage, vk::Format::eR8G8B8A8Unorm);

        // create sampler

        vk::SamplerCreateInfo samplerInfo;
        samplerInfo.magFilter = vk::Filter::eLinear;
        samplerInfo.minFilter = vk::Filter::eLinear;
        samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = vk::CompareOp::eAlways;
        samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;

        textureSampler = renderer.getDevice ().createSamplerUnique (samplerInfo);

    }


}

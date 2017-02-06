//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_TEXTURE_H
#define VULKANTEST_TEXTURE_H

#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"
#include "Renderer.h"
#include "TextureHelper.h"

namespace helper
{

    class Texture
    {
    public:
        Texture (helper::Renderer &renderer, std::string filename);

    private:
        std::string filename;
        helper::Renderer &renderer;

        vk::UniqueImage textureImage;
        vk::UniqueDeviceMemory textureImageMemory;
        vk::UniqueImageView textureImageView;
        vk::UniqueSampler textureSampler;
    };

}

#endif //VULKANTEST_TEXTURE_H

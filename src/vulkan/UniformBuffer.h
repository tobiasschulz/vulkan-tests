//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_UNIFORMBUFFER_H
#define VULKANTEST_UNIFORMBUFFER_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "helper/UniformBufferObject.h"
#include "helper/BufferHelper.h"
#include "Buffer.h"

namespace vulkan
{

    template<typename ubo_t>
    class UniformBuffer : public vulkan::Buffer
    {
    public:
        UniformBuffer (vulkan::Renderer *_renderer)
                : Buffer (_renderer, vk::BufferUsageFlagBits::eUniformBuffer)
        {
        }

        void create (ubo_t ubo)
        {
            vulkan::Buffer::create (sizeof (ubo), &ubo);
        }

        void update (ubo_t ubo)
        {
            vulkan::Buffer::update (sizeof (ubo), &ubo);
        }

    private:

    };

}

#endif //VULKANTEST_UNIFORMBUFFER_H

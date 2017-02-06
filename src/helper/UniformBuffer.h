//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_UNIFORMBUFFER_H
#define VULKANTEST_UNIFORMBUFFER_H

#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"
#include "../../3rdparty/glm/glm/gtc/matrix_transform.hpp"
#include "UniformBufferObject.h"
#include "BufferHelper.h"

namespace helper
{

    class UniformBuffer
    {
    public:
        UniformBuffer (helper::Renderer &renderer);

        void create ();
        void update ();

        vk::UniqueBuffer uniformStagingBuffer;
        vk::UniqueDeviceMemory uniformStagingBufferMemory;
        vk::UniqueBuffer uniformBuffer;
        vk::UniqueDeviceMemory uniformBufferMemory;

    private:
        helper::Renderer &renderer;

    };

}

#endif //VULKANTEST_UNIFORMBUFFER_H

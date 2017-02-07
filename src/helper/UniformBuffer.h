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
        UniformBuffer ();

        void create (helper::Renderer *renderer);
        void update (helper::Renderer *renderer, helper::UniformBufferObject ubo);

        vk::UniqueBuffer uniformStagingBuffer;
        vk::UniqueDeviceMemory uniformStagingBufferMemory;
        vk::UniqueBuffer uniformBuffer;
        vk::UniqueDeviceMemory uniformBufferMemory;

    private:

    };

}

#endif //VULKANTEST_UNIFORMBUFFER_H

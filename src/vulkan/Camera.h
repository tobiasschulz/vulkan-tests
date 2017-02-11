//
// Created by tobias on 07.02.17.
//

#ifndef VULKANTEST_CAMERA_H
#define VULKANTEST_CAMERA_H

#include <chrono>
#include "vulkan.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "helper/UniformBufferObject.h"
#include "helper/UniformBuffer.h"
#include "helper/BufferHelper.h"

namespace vulkan
{

    class Camera
    {
    public:
        Camera ();

        void update (vulkan::Renderer *renderer);
        helper::UniformBufferObject getUniformBufferObject ();

        void handleKeypress (int key, int action, int mods);

    private:
        helper::UniformBufferObject ubo;

        glm::vec3 cameraPosition;
        glm::vec3 cameraDirection;
        glm::vec3 cameraUp;
        float speed;
    };

}

#endif //VULKANTEST_CAMERA_H

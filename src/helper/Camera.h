//
// Created by tobias on 07.02.17.
//

#ifndef VULKANTEST_CAMERA_H
#define VULKANTEST_CAMERA_H

#include <chrono>
#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"
#include "../../3rdparty/glm/glm/gtc/matrix_transform.hpp"
#include "UniformBufferObject.h"
#include "UniformBuffer.h"
#include "BufferHelper.h"

namespace helper
{

    class Camera
    {
    public:
        Camera ();

        void update (helper::Renderer *renderer);
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

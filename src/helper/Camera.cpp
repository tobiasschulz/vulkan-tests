//
// Created by tobias on 07.02.17.
//

#include "Camera.h"

namespace helper
{

    Camera::Camera ()
    {
        ubo = {};
        cameraPosition = glm::vec3 (0.0f, 0.0f, 3.0f);
        cameraFront = glm::vec3 (0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3 (0.0f, 1.0f, 0.0f);
    }

    helper::UniformBufferObject Camera::getUniformBufferObject ()
    {
        return ubo;
    }

    void Camera::update (helper::Renderer *renderer)
    {
        static auto startTime = std::chrono::high_resolution_clock::now ();

        auto currentTime = std::chrono::high_resolution_clock::now ();
        float time = std::chrono::duration_cast<std::chrono::milliseconds> (currentTime - startTime).count () / 1000.0f;

        ubo.model = glm::rotate (glm::mat4 (), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective (glm::radians (45.0f), renderer->getSwapChainExtent ().width / (float) renderer->getSwapChainExtent ().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
    }
}
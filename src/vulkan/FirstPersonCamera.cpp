//
// Created by tobias on 07.02.17.
//

#include <iostream>
#include "helper/Vertex.h"
#include "FirstPersonCamera.h"

namespace vulkan
{

    FirstPersonCamera::FirstPersonCamera ()
    {
        ubo = {};
        cameraPosition = glm::vec3 (1.0f, 1.0f, 1.0f) * 10.0f;
        cameraDirection = glm::normalize (-cameraPosition); //glm::vec3 (-1.0f, -1.0f, -1.0f);
        cameraUp = glm::vec3 (0.0f, 1.0f, 0.0f);
        speed = 0.10f;
    }

    FirstPersonCamera::UniformBufferObject FirstPersonCamera::getUniformBufferObject ()
    {
        return ubo;
    }

    float yaw = 3.8;
    float pitch  = -0.6;
    bool mouseSet = false;

    void FirstPersonCamera::handleMouseMove (vulkan::Renderer *renderer, int x, int y)
    {
        if (!mouseSet) {
            vk::Extent2D windowSize = renderer->getSurface ()->getSize ();
            GLfloat centerX, centerY;
            centerX = windowSize.width / 2;
            centerY = windowSize.height / 2;

            GLfloat xoffset = x - centerX;
            GLfloat yoffset = y - centerY;
            GLfloat sensitivity = 800.0f;
            xoffset /= sensitivity;
            yoffset /= sensitivity;

            yaw += -xoffset;
            pitch += -yoffset;

            cameraDirection = glm::vec3 (cos (pitch) * sin (yaw), sin (pitch), cos (pitch) * cos (yaw));
            //std::cout << "cameraDirection: " << cameraDirection << ", yaw: " << yaw << ", pitch: " << pitch << std::endl;

            mouseSet = true;
            glfwSetCursorPos (renderer->getWindow ()->getNativeWindow (), centerX, centerY);
        }
        else {
            mouseSet = false;
        }
    }


    void FirstPersonCamera::update (vulkan::Renderer *renderer)
    {
        // static auto startTime = std::chrono::high_resolution_clock::now ();
        // auto currentTime = std::chrono::high_resolution_clock::now ();
        // float time = std::chrono::duration_cast<std::chrono::milliseconds> (currentTime - startTime).count () / 1000.0f;
        // glm::rotate (glm::mat4 (), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f));

        vk::Extent2D windowSize = renderer->getSurface ()->getSize ();

        ubo.model = glm::mat4 ();
        ubo.view = glm::lookAt (cameraPosition, cameraPosition + cameraDirection, cameraUp);
        ubo.proj = glm::perspective (glm::radians (45.0f), (float) windowSize.width / (float) windowSize.height, 0.1f, 1000.0f);
        ubo.proj[1][1] *= -1;

    }

    void FirstPersonCamera::handleKeypress (int key, int action, int mods)
    {
        if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
            cameraPosition += cameraDirection * speed;
        }
        else if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
            cameraPosition -= cameraDirection * speed;
        }
        else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
            cameraPosition -= glm::cross (cameraDirection, cameraUp) * speed;
        }
        else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
            cameraPosition += glm::cross (cameraDirection, cameraUp) * speed;
        }
        else if (key == GLFW_KEY_R && action != GLFW_RELEASE) {
            cameraPosition += cameraUp * speed;
        }
        else if (key == GLFW_KEY_F && action != GLFW_RELEASE) {
            cameraPosition -= cameraUp * speed;
        }
        else {
        }
    }

}
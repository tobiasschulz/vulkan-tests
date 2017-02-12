//
// Created by tobias on 12.02.17.
//

#ifndef VULKANTEST_WINDOW_H
#define VULKANTEST_WINDOW_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "SwapChain.h"
#include "helper/NonCopyable.h"
#include "Renderer.h"

namespace vulkan
{

    class Window
    {
    public:
        Window();
        void create(int desiredWidth, int desiredHeight);

        GLFWwindow* getNativeWindow();

    private:
        GLFWwindow *window;

    };

}

#endif //VULKANTEST_WINDOW_H

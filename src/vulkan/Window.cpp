//
// Created by tobias on 12.02.17.
//

#include "Window.h"

namespace vulkan
{


    Window::Window ()
    {

    }

    void Window::create (int desiredWidth, int desiredHeight)
    {
        glfwInit ();

        glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow (desiredWidth, desiredHeight, "Vulkan", nullptr, nullptr);


    }

    GLFWwindow *Window::getNativeWindow ()
    {
        return window;
    }

}

//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_VERTEX_H
#define VULKANTEST_VERTEX_H

#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"

namespace helper
{

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription ();
        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions ();
    };

}

#endif //VULKANTEST_VERTEX_H

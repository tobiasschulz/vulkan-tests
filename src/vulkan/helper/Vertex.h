//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_VERTEX_H
#define VULKANTEST_VERTEX_H

#include "../vulkan.h"

namespace helper
{

    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription ();
        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions ();

        Vertex ();
        Vertex (glm::vec3 pos, glm::vec3 color = glm::vec3 (), glm::vec2 texCoord = glm::vec3 ());

        Vertex &operator+= (const Vertex &rhs);

        friend Vertex operator+ (Vertex lhs, const Vertex &rhs);
    };

}

std::ostream &operator<< (std::ostream &out, const std::vector<helper::Vertex> &v);
std::ostream &operator<< (std::ostream &out, const helper::Vertex &v);
std::ostream &operator<< (std::ostream &out, const glm::vec4 &v);
std::ostream &operator<< (std::ostream &out, const glm::vec3 &v);
std::ostream &operator<< (std::ostream &out, const glm::vec2 &v);

#endif //VULKANTEST_VERTEX_H

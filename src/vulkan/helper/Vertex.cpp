//
// Created by tobias on 05.02.17.
//

#include "Vertex.h"
#include <iostream>
#include <iomanip>

namespace helper
{

    Vertex::Vertex ()
            : pos (), color (), texCoord ()
    {
    }

    Vertex::Vertex (glm::vec3 pos, glm::vec3 color, glm::vec2 texCoord)
            : pos (pos), color (color), texCoord (texCoord)
    {
    }

    Vertex &Vertex::operator+= (const Vertex &rhs)
    {
        pos += rhs.pos;
        color += rhs.color;
        texCoord += rhs.texCoord;
        return *this;
    }

    Vertex operator+ (Vertex lhs, const Vertex &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    std::ostream &operator<< (std::ostream &out, const std::vector<Vertex> &v)
    {
        out << "{ ";
        for (size_t i = 0; i < v.size (); i++) {
            if (i != 0) out << ", ";
            out << v[i];
        }
        out << " }";
        return out;
    }

    std::ostream &operator<< (std::ostream &out, const Vertex &v)
    {
        out << "[";
        std::stringstream ss;
        ss << v.pos;
        out << std::setw(24) << std:: right << ss.str () << ", ";
        ss.str(std::string());
        ss << v.color;
        out << std::setw(12) << std:: right << ss.str () << ", ";
        ss.str(std::string());
        ss << v.texCoord;
        out << std::setw(8) << std:: right << ss.str ();
        out << "]";
        return out;
        // return out << "(" << v.pos << ", " << v.color << ", " << v.texCoord << ")";
    }

    std::ostream &operator<< (std::ostream &out, const glm::vec4 &v)
    {
        return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    }

    std::ostream &operator<< (std::ostream &out, const glm::vec3 &v)
    {
        return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    std::ostream &operator<< (std::ostream &out, const glm::vec2 &v)
    {
        return out << "(" << v.x << ", " << v.y << ")";
    }

    vk::VertexInputBindingDescription Vertex::getBindingDescription ()
    {
        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof (Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        return bindingDescription;
    }

    std::array<vk::VertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions ()
    {
        std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[0].offset = offsetof (Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof (Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
        attributeDescriptions[2].offset = offsetof (Vertex, texCoord);

        return attributeDescriptions;
    }

}

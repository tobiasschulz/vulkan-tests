//
// Created by tobias on 05.02.17.
//

#include "Mesh.h"

namespace vulkan
{
    const std::vector<helper::Vertex> vertices = {
            {{ -1.0f,  0.1f, -1.0f },  { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
            {{ 1.0f,   0.1f, -1.0f },  { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
            {{ 1.0f,   0.1f, 1.0f },   { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
            {{ -1.0f,  0.1f, 1.0f },   { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},


            {{ -10.0f, 0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
            {{ 10.0f,  0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
            {{ 10.0f,  0.0f, 10.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
            {{ -10.0f, 0.0f, 10.0f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }},
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0,
            2, 1, 0, 0, 3, 2,

            4, 5, 6, 6, 7, 4,
            6, 5, 4, 4, 7, 6
    };


    Mesh::Mesh (vulkan::Renderer *_renderer)
            : renderer (_renderer),
              vertexBuffer(_renderer, vk::BufferUsageFlagBits::eVertexBuffer),
              indexBuffer(_renderer, vk::BufferUsageFlagBits::eIndexBuffer)
    {
    }

    void Mesh::create ()
    {
        // create vertex buffer
        vertexBuffer.create (
                sizeof (vertices[0]) * vertices.size (), // buffer size
                vertices.data()
        );
        // create vertex buffer
        indexBuffer.create (
                sizeof (indices[0]) * indices.size (), // buffer size
                indices.data()
        );
    }

    void Mesh::update (vulkan::Camera *camera)
    {
        // update vertex buffer
        vertexBuffer.update (
                sizeof (vertices[0]) * vertices.size (), // buffer size
                vertices.data()
        );
        // update vertex buffer
        indexBuffer.update (
                sizeof (indices[0]) * indices.size (), // buffer size
                indices.data()
        );
    }

    vk::Buffer Mesh::getVertexBuffer ()
    {
        return vertexBuffer.getBuffer ();
    }

    vk::Buffer Mesh::getIndexBuffer ()
    {
        return indexBuffer.getBuffer ();
    }

    uint32_t Mesh::getVertexCount ()
    {
        return (uint32_t) vertices.size ();
    }

    uint32_t Mesh::getIndexCount ()
    {
        return (uint32_t) indices.size ();
    }

}

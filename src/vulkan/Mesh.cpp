//
// Created by tobias on 05.02.17.
//

#include <iostream>
#include "Mesh.h"

namespace vulkan
{

    Mesh::Mesh (vulkan::Renderer *_renderer)
            : renderer (_renderer),
              vertexBuffer (_renderer, vk::BufferUsageFlagBits::eVertexBuffer),
              indexBuffer (_renderer, vk::BufferUsageFlagBits::eIndexBuffer)
    {
        vertices.push_back ({});
    }

    void Mesh::create ()
    {
        // create vertex buffer
        vertexBuffer.create (
                vertices.size () == 0 ? 0 : (sizeof (vertices[0]) * vertices.size ()), // buffer size
                vertices.data ()
        );
        // create vertex buffer
        indexBuffer.create (
                indices.size () == 0 ? 0 : (sizeof (indices[0]) * indices.size ()), // buffer size
                indices.data ()
        );
    }

    void Mesh::update (vulkan::FirstPersonCamera *camera)
    {
        update ();
    }

    std::vector<helper::Vertex> verticesTransformed;
    std::vector<uint16_t> indicesTransformed;

    void Mesh::update ()
    {
        if (modelMatrix == nullptr) {
            update (&vertices, &indices);
        }
        else {
            verticesTransformed.clear ();
            for (int i = 0; i < vertices.size (); i++) {
                auto vertex = vertices[i];
                glm::vec4 v = { vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f };
                v = (*modelMatrix) * v;
                vertex.pos = { v.x, v.y, v.z };
                //vertex.pos = { v.x + 5, v.y + 1, v.z + 4 };
                //std::cout << v.w << ", ";
                verticesTransformed.push_back (vertex);

                std::cout << vertex.pos.x << " " << vertex.pos.y << " " << vertex.pos.z << ", ";
            }
            std::cout << std::endl;

            indicesTransformed.clear ();
            for (int i = 0; i < indices.size (); i++) {
                auto index = indices[i];
                std::cout << index << ", ";
                indicesTransformed.push_back(index);
            }
            std::cout << std::endl;

            update (&verticesTransformed, &indicesTransformed);
        }
    }

    void Mesh::update (std::vector<helper::Vertex> *_vertices, std::vector<uint16_t> *_indices)
    {
        // update vertex buffer
        vertexBuffer.update (
                _vertices->size () == 0 ? 0 : (sizeof (_vertices[0]) * _vertices->size ()), // buffer size
                _vertices->data ()
        );
        // update vertex buffer
        indexBuffer.update (
                _indices->size () == 0 ? 0 : (sizeof (_indices[0]) * _indices->size ()), // buffer size
                _indices->data ()
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

    void Mesh::setModelMatrix (glm::mat4 m)
    {
        modelMatrix = std::make_unique<glm::mat4> (m);
    }

    void Mesh::clearModelMatrix ()
    {
        modelMatrix.reset ();
    }

    glm::mat4 Mesh::getModelMatrix ()
    {
        return *modelMatrix;
    }

    void Mesh::add (std::vector<helper::Vertex> verticesNew, std::vector<uint16_t> indicesNew)
    {
        unsigned short vertexbuffer_size = (unsigned short) vertices.size ();
        for (size_t i = 0; i < indicesNew.size (); i++) {
            indices.push_back (indicesNew[i] + vertexbuffer_size);
        }

        vertices.insert (std::end (vertices), std::begin (verticesNew), std::end (verticesNew));
    }

    template<size_t N, size_t M>
    void Mesh::add (std::array<helper::Vertex, N> verticesNew, std::array<uint16_t, M> indicesNew)
    {
        unsigned short vertexbuffer_size = (unsigned short) vertices.size ();
        for (size_t i = 0; i < indicesNew.size (); i++) {
            indices.push_back (indicesNew[i] + vertexbuffer_size);
        }

        vertices.insert (std::end (vertices), std::begin (verticesNew), std::end (verticesNew));
    };
}

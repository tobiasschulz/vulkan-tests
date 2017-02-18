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
                sizeof (helper::Vertex) * vertices.size (), // buffer size
                vertices.data ()
        );
        // create vertex buffer
        indexBuffer.create (
                sizeof (uint16_t) * indices.size (), // buffer size
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
                verticesTransformed.push_back (vertex);
                //std::cout << v.x << " " << v.y << " " << v.z << " ; ";
            }
            //std::cout << std::endl;

            update (&verticesTransformed, &indices);
        }
    }

    void Mesh::update (std::vector<helper::Vertex> *_vertices, std::vector<uint16_t> *_indices)
    {
        return;
        // update vertex buffer
        vertexBuffer.update (
                sizeof (helper::Vertex) * _vertices->size (), // buffer size
                _vertices->data ()
        );
        // update vertex buffer
        indexBuffer.update (
                sizeof (uint16_t) * _indices->size (), // buffer size
                _indices->data ()
        );
    }

    std::vector<helper::Vertex> Mesh::getVertexVector ()
    {
        return vertices;
    }

    std::vector<uint16_t> Mesh::getIndexVector ()
    {
        return indices;
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

    void Mesh::addRectangle (helper::Vertex min, helper::Vertex max)
    {
        static const std::vector<uint16_t> indicesNew = {
                0, 1, 2, 2, 3, 0,
                2, 1, 0, 0, 3, 2,
        };


        if (min.pos.y == max.pos.y) {
            const std::vector<helper::Vertex> verticesNewY = {
                    {{ min.pos.x, min.pos.y, min.pos.z }, { min.color.x, min.color.y, min.color.z }, { min.texCoord.x, min.texCoord.y }},
                    {{ max.pos.x, min.pos.y, min.pos.z }, { max.color.x, min.color.y, min.color.z }, { max.texCoord.x, min.texCoord.y }},
                    {{ max.pos.x, min.pos.y, max.pos.z }, { max.color.x, min.color.y, max.color.z }, { max.texCoord.x, max.texCoord.y }},
                    {{ min.pos.x, min.pos.y, max.pos.z }, { min.color.x, min.color.y, max.color.z }, { min.texCoord.x, max.texCoord.y }},
            };
            //std::cout << verticesNewY << std::endl;
            add (verticesNewY, indicesNew);
        }
        else if (min.pos.x == max.pos.x) {
            const std::vector<helper::Vertex> verticesNewX = {
                    {{ min.pos.x, min.pos.y, min.pos.z }, { min.color.x, min.color.y, min.color.z }, { min.texCoord.x, min.texCoord.y }},
                    {{ min.pos.x, max.pos.y, min.pos.z }, { max.color.x, min.color.y, min.color.z }, { max.texCoord.x, min.texCoord.y }},
                    {{ min.pos.x, max.pos.y, max.pos.z }, { max.color.x, min.color.y, max.color.z }, { max.texCoord.x, max.texCoord.y }},
                    {{ min.pos.x, min.pos.y, max.pos.z }, { min.color.x, min.color.y, max.color.z }, { min.texCoord.x, max.texCoord.y }},
            };
            add (verticesNewX, indicesNew);
        }
        else if (min.pos.z == max.pos.z) {
            const std::vector<helper::Vertex> verticesNewZ = {
                    {{ min.pos.x, min.pos.y, min.pos.z }, { min.color.x, min.color.y, min.color.z }, { min.texCoord.x, min.texCoord.y }},
                    {{ max.pos.x, min.pos.y, min.pos.z }, { max.color.x, min.color.y, min.color.z }, { max.texCoord.x, min.texCoord.y }},
                    {{ max.pos.x, max.pos.y, min.pos.z }, { max.color.x, min.color.y, max.color.z }, { max.texCoord.x, max.texCoord.y }},
                    {{ min.pos.x, max.pos.y, min.pos.z }, { min.color.x, min.color.y, max.color.z }, { min.texCoord.x, max.texCoord.y }},
            };
            add (verticesNewZ, indicesNew);
        }
    }
}

//
// Created by tobias on 05.02.17.
//

#include "Mesh.h"

namespace helper
{
    const std::vector<helper::Vertex> vertices = {
            {{ -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
            {{ 0.5f,  -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
            {{ 0.5f,  0.5f },  { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
            {{ -0.5f, 0.5f },  { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }}
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };


    Mesh::Mesh (helper::Renderer &_renderer)
            : renderer (_renderer)
    {
        vk::Device device = renderer.getDevice ();

        // create vertex buffer
        {
            VkDeviceSize bufferSize = sizeof (vertices[0]) * vertices.size ();

            vk::Buffer stagingBuffer;
            vk::DeviceMemory stagingBufferMemory;
            std::tie (stagingBuffer, stagingBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlagBits::eTransferSrc,
                    vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            void *data = device.mapMemory (stagingBufferMemory, 0, bufferSize);
            memcpy (data, vertices.data (), (size_t) bufferSize);
            device.unmapMemory (stagingBufferMemory);

            vk::Buffer _vertexBuffer;
            vk::DeviceMemory _vertexBufferMemory;
            std::tie (_vertexBuffer, _vertexBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eVertexBuffer,
                    vk::MemoryPropertyFlagBits::eDeviceLocal
            );
            vertexBuffer = vk::UniqueBuffer (_vertexBuffer);
            vertexBufferMemory = vk::UniqueDeviceMemory (_vertexBufferMemory);

            helper::BufferHelper::copyBuffer (renderer, stagingBuffer, *vertexBuffer, bufferSize);
        }

        // create index buffer
        {
            vk::DeviceSize bufferSize = sizeof (indices[0]) * indices.size ();

            vk::Buffer stagingBuffer;
            vk::DeviceMemory stagingBufferMemory;
            std::tie (stagingBuffer, stagingBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlagBits::eTransferSrc,
                    vk::MemoryPropertyFlags (vk::MemoryPropertyFlagBits::eHostVisible) | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            void *data = device.mapMemory (stagingBufferMemory, 0, bufferSize);
            memcpy (data, indices.data (), (size_t) bufferSize);
            device.unmapMemory (stagingBufferMemory);

            vk::Buffer _indexBuffer;
            vk::DeviceMemory _indexBufferMemory;
            std::tie (_indexBuffer, _indexBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eIndexBuffer,
                    vk::MemoryPropertyFlagBits::eDeviceLocal
            );
            indexBuffer = vk::UniqueBuffer (_indexBuffer);
            indexBufferMemory = vk::UniqueDeviceMemory (_indexBufferMemory);

            helper::BufferHelper::copyBuffer (renderer, stagingBuffer, *indexBuffer, bufferSize);
        }
    }


    vk::Buffer Mesh::getVertexBuffer ()
    {
        return *vertexBuffer;
    }

    vk::Buffer Mesh::getIndexBuffer ()
    {
        return *indexBuffer;
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

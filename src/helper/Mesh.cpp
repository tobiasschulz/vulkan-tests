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

            std::tie (vertexBuffer, vertexBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eVertexBuffer,
                    vk::MemoryPropertyFlagBits::eDeviceLocal
            );

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

            std::tie (indexBuffer, indexBufferMemory) = helper::BufferHelper::createBuffer (
                    renderer,
                    bufferSize,
                    vk::BufferUsageFlags (vk::BufferUsageFlagBits::eTransferDst) | vk::BufferUsageFlagBits::eIndexBuffer,
                    vk::MemoryPropertyFlagBits::eDeviceLocal
            );

            helper::BufferHelper::copyBuffer (renderer, stagingBuffer, *vertexBuffer, bufferSize);
        }
    }

}

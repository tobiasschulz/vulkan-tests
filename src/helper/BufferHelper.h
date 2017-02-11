//
// Created by tobias on 06.02.17.
//

#ifndef VULKANTEST_BUFFERHELPER_H
#define VULKANTEST_BUFFERHELPER_H


#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"
#include "Renderer.h"
#include "Vertex.h"

namespace helper
{

    class BufferHelper
    {
    public:
        static std::pair<vk::Buffer, vk::DeviceMemory> createBuffer (helper::Renderer *renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
        static std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> createBufferUnique (helper::Renderer *renderer, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
        static void copyBuffer (Renderer *renderer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
    };

}

#endif //VULKANTEST_BUFFERHELPER_H

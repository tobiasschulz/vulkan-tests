//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_MESH_H
#define VULKANTEST_MESH_H

#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "BufferHelper.h"

namespace helper
{

    class Mesh
    {
    public:
        Mesh (helper::Renderer &renderer);

        vk::Buffer getVertexBuffer ();
        uint32_t getVertexCount ();
        vk::Buffer getIndexBuffer ();
        uint32_t getIndexCount ();

    private:
        helper::Renderer &renderer;

        vk::UniqueBuffer vertexBuffer;
        vk::UniqueDeviceMemory vertexBufferMemory;
        vk::UniqueBuffer indexBuffer;
        vk::UniqueDeviceMemory indexBufferMemory;

    };

}

#endif //VULKANTEST_MESH_H

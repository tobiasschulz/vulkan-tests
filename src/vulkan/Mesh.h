//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_MESH_H
#define VULKANTEST_MESH_H

#include "vulkan.h"
#include "glm/glm.hpp"
#include "Renderer.h"
#include "helper/Vertex.h"
#include "helper/BufferHelper.h"
#include "FirstPersonCamera.h"
#include "Buffer.h"

namespace vulkan
{

    class Mesh
    {
    public:
        Mesh (vulkan::Renderer *renderer);

        void create ();
        void update (vulkan::FirstPersonCamera* camera);

        vk::Buffer getVertexBuffer ();
        uint32_t getVertexCount ();
        vk::Buffer getIndexBuffer ();
        uint32_t getIndexCount ();

    private:
        vulkan::Renderer *renderer;

        vulkan::Buffer vertexBuffer;
        vulkan::Buffer indexBuffer;

    };

}

#endif //VULKANTEST_MESH_H

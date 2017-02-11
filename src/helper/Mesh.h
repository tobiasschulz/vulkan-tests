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
#include "Camera.h"
#include "Buffer.h"

namespace helper
{

    class Mesh
    {
    public:
        Mesh (helper::Renderer *renderer);

        void create ();
        void update (helper::Camera* camera);

        vk::Buffer getVertexBuffer ();
        uint32_t getVertexCount ();
        vk::Buffer getIndexBuffer ();
        uint32_t getIndexCount ();

    private:
        helper::Renderer *renderer;

        helper::Buffer vertexBuffer;
        helper::Buffer indexBuffer;

    };

}

#endif //VULKANTEST_MESH_H

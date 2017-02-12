//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_MESH_H
#define VULKANTEST_MESH_H

#include "vulkan.h"
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

        virtual void create ();
        virtual void update ();
        virtual void update (vulkan::FirstPersonCamera *camera);
        void update (std::vector<helper::Vertex> *vertices, std::vector<uint16_t> *indices);

        void add (std::vector<helper::Vertex> verticesNew, std::vector<uint16_t> indicesNew);
        template<size_t N, size_t M>
        void add (std::array<helper::Vertex, N> verticesNew, std::array<uint16_t, M> indicesNew);

        vk::Buffer getVertexBuffer ();
        uint32_t getVertexCount ();
        vk::Buffer getIndexBuffer ();
        uint32_t getIndexCount ();

        void setModelMatrix(glm::mat4 m);
        void clearModelMatrix ();
        glm::mat4 getModelMatrix();

    private:
        vulkan::Renderer *renderer;

        std::unique_ptr<glm::mat4> modelMatrix;

        std::vector<helper::Vertex> vertices;
        std::vector<uint16_t> indices;

        vulkan::Buffer vertexBuffer;
        vulkan::Buffer indexBuffer;
    };

}

#endif //VULKANTEST_MESH_H

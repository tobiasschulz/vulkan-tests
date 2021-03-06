//
// Created by tobias on 12.02.17.
//

#include "RotatingMesh.h"

namespace effects
{

    RotatingMesh::RotatingMesh (vulkan::Renderer *_renderer)
            : Mesh (_renderer)
    {
    }

    void RotatingMesh::update (vulkan::FirstPersonCamera *camera)
    {
        static auto startTime = std::chrono::high_resolution_clock::now ();
        auto currentTime = std::chrono::high_resolution_clock::now ();
        float time = std::chrono::duration_cast<std::chrono::milliseconds> (currentTime - startTime).count () / 1000.0f;

        glm::vec3 centerOfMesh;
        auto vertices = getVertexVector ();
        if (vertices.size () > 1) {
            for (int i = 1; i < vertices.size (); i++) {
                auto vertex = vertices[i];
                centerOfMesh += vertex.pos;
            }
            centerOfMesh /= vertices.size () - 1;
        }

        glm::mat4 rotate = glm::rotate (glm::mat4 (), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f));
        glm::mat4 m = glm::translate (glm::mat4 (), centerOfMesh) * rotate * glm::translate (glm::mat4 (), -centerOfMesh);
        //glm::mat4 m = glm::translate (glm::mat4 (), -centerOfMesh);

        /*m = glm::translate (glm::mat4(), {2,-1,0});

        static int k=0;
        k++;
        if (k%2==0)
        setModelMatrix (m);
        else
            clearModelMatrix ();*/
        setModelMatrix (m);

        vulkan::Mesh::update (camera);
    }


}

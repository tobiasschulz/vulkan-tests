//
// Created by tobias on 12.02.17.
//

#ifndef VULKANTEST_ROTATINGMESH_H
#define VULKANTEST_ROTATINGMESH_H

#include "../Mesh.h"

namespace effects
{

    class RotatingMesh : public vulkan::Mesh
    {
    public:
        RotatingMesh (vulkan::Renderer *renderer);

        void update (vulkan::FirstPersonCamera *camera);

    };

}

#endif //VULKANTEST_ROTATINGMESH_H

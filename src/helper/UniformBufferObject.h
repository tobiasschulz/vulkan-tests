//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_UNIFORMBUFFEROBJECT_H
#define VULKANTEST_UNIFORMBUFFEROBJECT_H

#include "vulkan.h"
#include "../../3rdparty/glm/glm/glm.hpp"

namespace helper
{

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

}

#endif //VULKANTEST_UNIFORMBUFFEROBJECT_H

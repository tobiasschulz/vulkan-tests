//
// Created by tobias on 05.02.17.
//

#ifndef VULKANTEST_SHADER_H
#define VULKANTEST_SHADER_H

#include <string>
#include <vector>
#include "vulkan.h"

namespace helper
{

    class Shader
    {
    public:
        vk::ShaderModule getShaderModule ();
        vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo (const char *name);

        static Shader loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, const std::string &&filename);
        static Shader loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code);

    private:
        Shader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code);
        vk::UniqueShaderModule shaderModule;
        vk::ShaderStageFlagBits stage;
    };

}

#endif //VULKANTEST_SHADER_H

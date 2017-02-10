//
// Created by tobias on 05.02.17.
//

#include "Shader.h"
#include "io.h"

namespace helper
{
    Shader Shader::loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, const std::string &&filename)
    {
        std::vector<char> code = helper::io::readFile (filename);
        return loadShader (device, stage, code);
    }

    Shader Shader::loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code)
    {
        return Shader (device, stage, code);
    }

    Shader::Shader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code)
    {
        this->stage = stage;
        vk::ShaderModuleCreateInfo createInfo ({}, code.size (), (uint32_t *) code.data ());

        this->shaderModule = device.createShaderModuleUnique (createInfo);
    }

    vk::ShaderModule Shader::getShaderModule ()
    {
        return shaderModule.get ();
    }

    vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo (const char *name)
    {
        return vk::PipelineShaderStageCreateInfo ({}, stage, getShaderModule (), name);
    }
}

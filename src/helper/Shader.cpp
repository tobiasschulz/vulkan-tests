//
// Created by tobias on 05.02.17.
//

#include "Shader.h"
#include "io.h"

namespace helper
{
    Shader &&Shader::loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, const std::string &&filename)
    {
        std::vector<char> code = helper::io::readFile (filename);
        return loadShader (device, stage, code);
    }

    Shader &&Shader::loadShader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code)
    {
        return Shader (device, stage, code);
    }

    Shader::Shader (const vk::Device &device, vk::ShaderStageFlagBits stage, std::vector<char> code)
    {
        this->stage = stage;
        this->shaderModule = device.createShaderModuleUnique (vk::ShaderModuleCreateInfo ({}, code.size (), (uint32_t *) code.data ()));
    }

    vk::ShaderModule Shader::getShaderModule ()
    {
        return shaderModule.get ();
    }

    vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo (std::string name)
    {
        return vk::PipelineShaderStageCreateInfo ({}, stage, getShaderModule (), name.c_str ());
    }
}

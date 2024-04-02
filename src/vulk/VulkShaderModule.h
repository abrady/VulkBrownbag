#pragma once

#include "Vulk.h"

struct VulkShaderModule
{
    Vulk &vk;
    VkShaderModule shaderModule;
    explicit VulkShaderModule(Vulk &vk, std::string const &path)
        : vk(vk)
    {
        shaderModule = vk.createShaderModule(readFileIntoMem(path));
    }
    ~VulkShaderModule()
    {
        vkDestroyShaderModule(vk.device, shaderModule, nullptr);
    }
    static std::shared_ptr<VulkShaderModule> fromFile(Vulk &vk, std::string const &path)
    {
        return std::make_shared<VulkShaderModule>(vk, path);
    }
};

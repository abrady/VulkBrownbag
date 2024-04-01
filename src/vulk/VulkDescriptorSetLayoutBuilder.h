#pragma once

#include "ClassNonCopyableNonMovable.h"
#include "VulkDescriptorSetLayout.h"
#include "VulkUtil.h"

class VulkDescriptorSetLayoutBuilder
{
  Vulk &vk;

public:
  VulkDescriptorSetLayoutBuilder(Vulk &vk) : vk(vk)
  {
  }
  VulkDescriptorSetLayoutBuilder &addUniformBuffer(VkShaderStageFlags stageFlags, uint32_t binding);
  VulkDescriptorSetLayoutBuilder &addImageSampler(VkShaderStageFlags stageFlags, uint32_t binding);
  VulkDescriptorSetLayoutBuilder &addStorageBuffer(VkShaderStageFlags stageFlags, uint32_t binding);

  // and finally, build the layout
  std::shared_ptr<VulkDescriptorSetLayout> build();

private:
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> layoutBindingsMap;
};

//
// Created by user on 6/28/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanShaderAsset.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/28/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSHADERASSET_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSHADERASSET_H_
#include "Asset.h"

namespace GlassEye {
class VulkanShaderAsset : public Asset {
 public:
  // All shader paths are relative to the Project's shader folder as root
  static Ref <VulkanShaderAsset> Create(const std::filesystem::path &relativePath);

  VulkanShaderAsset(GUID id);
  ~VulkanShaderAsset();

  void Release();

  vk::ShaderModule GetShaderModule() const { return ShaderModule_; }
  vk::ShaderStageFlagBits GetStage() const { return Stage_; }
 private:
  vk::ShaderModule ShaderModule_ = {};
  vk::ShaderStageFlagBits Stage_ = {};

 private:
  static vk::ShaderModule CreateShaderModule(const std::vector<char> &shaderCode);
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSHADERASSET_H_

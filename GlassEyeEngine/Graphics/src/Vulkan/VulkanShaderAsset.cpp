//
// Created by user on 6/28/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanShaderAsset.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/28/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanShaderAsset.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "AssetManager.h"

vk::ShaderModule GlassEye::VulkanShaderAsset::CreateShaderModule(const std::vector<char> &shaderCode) {
  vk::ShaderModuleCreateInfo createInfo({},
                                        shaderCode.size(),
                                        reinterpret_cast<const uint32_t *>(shaderCode.data()));
  const VulkanContext &context = *VulkanContext::GetContext();
  return context.GetLogicalDevice()->GetDevice().createShaderModule(createInfo);
}
GlassEye::Ref<GlassEye::VulkanShaderAsset> GlassEye::VulkanShaderAsset::Create(const std::filesystem::path &relativePath) {
  return GlassEye::Ref<GlassEye::VulkanShaderAsset>();
}
GlassEye::VulkanShaderAsset::VulkanShaderAsset(GlassEye::GUID id) : Asset(id) {
  return AssetManager::Get().
}
GlassEye::VulkanShaderAsset::~VulkanShaderAsset() {
  Release();
}
void GlassEye::VulkanShaderAsset::Release() {

}

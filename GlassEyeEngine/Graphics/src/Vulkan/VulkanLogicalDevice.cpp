//
// Created by user on 6/18/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanLogicalDevice.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/18/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanUtility.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanPhysicalDevice.h"

GlassEye::VulkanLogicalDevice::VulkanLogicalDevice(VulkanContext &context,
                                                   const VulkanPhysicalDevice &physicalDevice,
                                                   vk::PhysicalDeviceFeatures features) : EnabledFeatures_(features),
                                                                                          PhysicalDevice_(&physicalDevice) {
  const VulkanUtility::QueueFamilyIndices &indices = physicalDevice.GetQueueFamilyIndices();

  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32> uniqueQueueFamilies = {indices.Graphics.value(),
                                          indices.Present.value()};
  float queuePriority = 1.0f;
  queueCreateInfos.reserve(uniqueQueueFamilies.size());
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    queueCreateInfos.emplace_back(vk::DeviceQueueCreateInfo({}, queueFamily, 1, &queuePriority));
  }

  vk::DeviceCreateInfo
      deviceCreateInfo({}, queueCreateInfos, context.GetEnabledLayers(), context.GetEnabledExtensions(),
                       &features);
  Device_ = physicalDevice.GetPhysicalDevice().createDevice(deviceCreateInfo);

  // Create Queues for presenting and graphics
  GraphicsQueue_ = Device_.getQueue(physicalDevice.GetQueueFamilyIndices().Graphics.value(), 0);
  PresentQueue_ = Device_.getQueue(physicalDevice.GetQueueFamilyIndices().Present.value(), 0);
}
GlassEye::VulkanLogicalDevice::~VulkanLogicalDevice() {
  Device_.destroy();
}
const vk::Device &GlassEye::VulkanLogicalDevice::GetDevice() const {
  return Device_;
}
const vk::Queue &GlassEye::VulkanLogicalDevice::GetGraphicsQueue() const {
  return GraphicsQueue_;
}
const vk::Queue &GlassEye::VulkanLogicalDevice::GetPresentQueue() const {
  return PresentQueue_;
}
const GlassEye::VulkanPhysicalDevice &GlassEye::VulkanLogicalDevice::GetPhysicalDevice() const {
  if (PhysicalDevice_ == nullptr) {
    GLASS_CORE_ERROR("Physical Device is Null!");
    GLASSEYE_BREAK();
  }
  return *PhysicalDevice_;
}

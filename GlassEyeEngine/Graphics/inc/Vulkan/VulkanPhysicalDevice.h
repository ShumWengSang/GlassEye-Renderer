//
// Created by user on 6/17/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanPhysicalDevice.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/17/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANPHYSICALDEVICE_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANPHYSICALDEVICE_H_
#include "Vulkan/VulkanUtility.h"
namespace GlassEye {

class VulkanPhysicalDevice {
 public:

  VulkanPhysicalDevice(const class VulkanContext &instance, const vk::SurfaceKHR &surface);
  ~VulkanPhysicalDevice() = default;

  const vk::PhysicalDevice &GetPhysicalDevice() const;
  const VulkanUtility::QueueFamilyIndices &GetQueueFamilyIndices() const;
  const VulkanUtility::SwapChainSupportDetails &GetSwapChainSupportDetails() const;
  vk::SampleCountFlagBits GetMsaaSamples() const;
 private:
  vk::PhysicalDevice ChooseBestPhysicalDevice(
      const std::vector<vk::PhysicalDevice> &AvailableDevices,
      const vk::SurfaceKHR &surface) const;
  bool CheckDeviceExtensionSupport(
      vk::PhysicalDevice const &device) const;
  bool CheckDeviceFeatures(vk::PhysicalDevice const &device) const;
  vk::SampleCountFlagBits GetMaxUsableSampleCount(vk::PhysicalDevice const &device) const;

  void LogPhysicalDevice();

 private:
  vk::PhysicalDevice PhysicalDevice_;

  /** Misc. Device properties that may be useful */
  vk::PhysicalDeviceProperties DeviceProperties_{};
  vk::PhysicalDeviceFeatures DeviceFeatures_{};
  vk::PhysicalDeviceMemoryProperties MemoryProperties_{};

  /** The max supported MSSA level on this device */
  vk::SampleCountFlagBits MsaaSamples_ = vk::SampleCountFlagBits::e1;

  VulkanUtility::QueueFamilyIndices QueueFamilyIndices_ = {};
  VulkanUtility::SwapChainSupportDetails SwapChainSupportDetails_ = {};

};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANPHYSICALDEVICE_H_

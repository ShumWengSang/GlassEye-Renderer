//
// Created by user on 6/18/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanLogicalDevice.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/18/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANLOGICALDEVICE_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANLOGICALDEVICE_H_
namespace GlassEye {
class VulkanLogicalDevice {
 public:
  VulkanLogicalDevice(
      class VulkanContext &context,
      const class VulkanPhysicalDevice &physicalDevice,
      vk::PhysicalDeviceFeatures features);
  ~VulkanLogicalDevice();
  const vk::Device &GetDevice() const;
  const vk::Queue &GetGraphicsQueue() const;
  const vk::Queue &GetPresentQueue() const;
  const VulkanPhysicalDevice &GetPhysicalDevice() const;
 private:
  vk::Device Device_ = nullptr;
  const class VulkanPhysicalDevice *PhysicalDevice_ = nullptr;
  vk::PhysicalDeviceFeatures EnabledFeatures_;

  /** Handle for the graphics queue */
  vk::Queue GraphicsQueue_ = nullptr;

  /** Handle to the presentation queue */
  vk::Queue PresentQueue_ = nullptr;
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANLOGICALDEVICE_H_

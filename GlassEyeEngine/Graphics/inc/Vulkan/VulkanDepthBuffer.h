//
// Created by user on 6/20/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanDepthBuffer.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/20/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_DEPTHBUFFER_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_DEPTHBUFFER_H_
namespace GlassEye {
class VulkanDepthBuffer {
 public:
  VulkanDepthBuffer(
      class VulkanContext &context,
      class VulkanPhysicalDevice &physicalDevice,
      class VulkanLogicalDevice &logicalDevice,
      vk::SampleCountFlagBits sampleCount,
      vk::Extent2D extent);
  ~VulkanDepthBuffer();

  vk::Format FindDepthFormat();

  void Create();
  void Create(vk::Extent2D extent);
  void Destroy();

  const vk::Image &GetImage() const;
  const vk::DeviceMemory &GetMemory() const;
  const vk::ImageView &GetImageView() const;
  const vk::Format &GetFormat() const;

 private:
  const VulkanLogicalDevice *Device_;
  const VulkanPhysicalDevice *PhysicalDevice_;
  const VulkanContext *Context_;
  vk::Image Image_{};
  vk::ImageView ImageView_{};
  vk::DeviceMemory Memory_{};
  vk::Format Format_{};
  vk::Extent2D Extents_{};
  vk::SampleCountFlagBits SampleCount_ = vk::SampleCountFlagBits::e1;

 private:
  void CreateImage();
  void CreateImageView();
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_DEPTHBUFFER_H_

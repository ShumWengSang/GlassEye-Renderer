//
// Created by user on 6/20/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanDepthBuffer.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/20/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanDepthBuffer.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanUtility.h"
#include "Vulkan/VulkanContext.h"
#include "Logger.h"

GlassEye::VulkanDepthBuffer::VulkanDepthBuffer(VulkanContext &context,
                                               VulkanPhysicalDevice &physicalDevice,
                                               VulkanLogicalDevice &logicalDevice,
                                               vk::SampleCountFlagBits sampleCount,
                                               vk::Extent2D extent) :
    Context_(&context),
    Device_(&logicalDevice),
    PhysicalDevice_(&physicalDevice),
    Extents_(extent),
    SampleCount_(sampleCount) {
  Create();
}
GlassEye::VulkanDepthBuffer::~VulkanDepthBuffer() {
  Destroy();
}

void GlassEye::VulkanDepthBuffer::Create() {

  if (!Image_ && !Memory_ && !ImageView_) {
    GLASS_CORE_ERROR("Image/ImageView/Memory is not null! They must be to create!");
    GLASSEYE_BREAK();
  }
  Format_ = FindDepthFormat();

  CreateImage();
  CreateImageView();
  // Transition Image
  Context_->TransitionImageLayout(Image_,
                                  Format_,
                                  vk::ImageLayout::eUndefined,
                                  vk::ImageLayout::eDepthStencilAttachmentOptimal);
}
void GlassEye::VulkanDepthBuffer::Destroy() {
  Device_->GetDevice().destroy(Image_);
  Image_ = nullptr;
  Device_->GetDevice().destroy(ImageView_);
  ImageView_ = nullptr;
  Device_->GetDevice().freeMemory(Memory_);
  Memory_ = nullptr;
}
const vk::Image &GlassEye::VulkanDepthBuffer::GetImage() const {
  return Image_;
}
const vk::DeviceMemory &GlassEye::VulkanDepthBuffer::GetMemory() const {
  return Memory_;
}
const vk::ImageView &GlassEye::VulkanDepthBuffer::GetImageView() const {
  return ImageView_;
}
const vk::Format &GlassEye::VulkanDepthBuffer::GetFormat() const {
  return Format_;
}
void GlassEye::VulkanDepthBuffer::CreateImage() {
  VulkanUtility::CreateImage(
      Device_->GetDevice(),
      PhysicalDevice_->GetPhysicalDevice(),
      Extents_.width,
      Extents_.height,
      /* Format */ Format_,
      /* Tiling */ vk::ImageTiling::eOptimal,
      /* Usage */ vk::ImageUsageFlagBits::eDepthStencilAttachment,
      /* Props */ vk::MemoryPropertyFlagBits::eDeviceLocal,
      Image_,
      Memory_,
      SampleCount_
  );
}

void GlassEye::VulkanDepthBuffer::CreateImageView() {
  ImageView_ = VulkanUtility::CreateImageView(
      Device_->GetDevice(), Image_, Format_, vk::ImageAspectFlagBits::eDepth);
}

vk::Format GlassEye::VulkanDepthBuffer::FindDepthFormat() {
  return VulkanUtility::FindSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint,
                                             vk::Format::eD24UnormS8Uint},
                                            vk::ImageTiling::eOptimal,
                                            vk::FormatFeatureFlagBits::eDepthStencilAttachment,
                                            PhysicalDevice_->GetPhysicalDevice());
}
void GlassEye::VulkanDepthBuffer::Create(vk::Extent2D extent) {
  Extents_ = extent;
  Create();
}

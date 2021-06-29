//
// Created by user on 6/18/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanSwapChain.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/18/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanUtility.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanPhysicalDevice.h"

GlassEye::VulkanSwapChain::VulkanSwapChain(const vk::Extent2D &extent,
                                           const GlassEye::VulkanLogicalDevice &logicalDevice,
                                           const GlassEye::VulkanPhysicalDevice &physicalDevice,
                                           const vk::SurfaceKHR &surface)
    : Extent_(extent), LogicalDevice_(&logicalDevice) {
  VulkanUtility::SwapChainSupportDetails swap_chain_support_details = physicalDevice.GetSwapChainSupportDetails();

  vk::SurfaceFormatKHR surfaceFormat = ChooseSwapChainSurfaceFormat(swap_chain_support_details.formats);
  vk::PresentModeKHR presentMode = ChooseSwapChainPresentMode(swap_chain_support_details.presentModes);

  /* Always get one more then min image, and check against max*/
  uint32 imageCount = swap_chain_support_details.capabilities.minImageCount + 1;
  if (swap_chain_support_details.capabilities.maxImageCount > 0
      && imageCount > swap_chain_support_details.capabilities.maxImageCount) {
    imageCount = swap_chain_support_details.capabilities.maxImageCount;
  }

  // Create the swap chain object creation information
  vk::SwapchainCreateInfoKHR swapChainCreateInfo(
      {},
      surface,
      imageCount,
      surfaceFormat.format,
      surfaceFormat.colorSpace,
      Extent_,
      1,
      vk::ImageUsageFlagBits::eColorAttachment,
      vk::SharingMode::eExclusive,
      {},
      swap_chain_support_details.capabilities.currentTransform,
      vk::CompositeAlphaFlagBitsKHR::eOpaque,
      presentMode,
      true,
      nullptr
  );

  uint32 graphicsQueueIndex = physicalDevice.GetQueueFamilyIndices().Graphics.value();
  uint32 presentQueueFamilyIndex = physicalDevice.GetQueueFamilyIndices().Present.value();
  uint32 queueFamilyIndices[2] = {graphicsQueueIndex, presentQueueFamilyIndex};
  if (graphicsQueueIndex != presentQueueFamilyIndex) {
    // If the graphics and present queues are from different queue families, we either have to explicitly transfer
    // ownership of images between the queues, or we have to create the swapchain with imageSharingMode as
    // vk::SharingMode::eConcurrent
    swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    swapChainCreateInfo.queueFamilyIndexCount = 2;
    swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  }

  SwapChain_ = logicalDevice.GetDevice().createSwapchainKHR(swapChainCreateInfo);

  ///////////////// Setup Image and Image Views /////////////////////////////
  std::vector<vk::Image> Images = logicalDevice.GetDevice().getSwapchainImagesKHR(SwapChain_);

  vk::ComponentMapping componentMapping(
      vk::ComponentSwizzle::eR,
      vk::ComponentSwizzle::eG,
      vk::ComponentSwizzle::eB,
      vk::ComponentSwizzle::eA);
  vk::ImageSubresourceRange subResourceRange(
      vk::ImageAspectFlagBits::eColor,
      0,
      1,
      0,
      1);

  Buffers_.reserve(Images.size());
  for (auto image : Images) {
    vk::ImageViewCreateInfo imageViewCreateInfo(
        vk::ImageViewCreateFlags(),
        image,
        vk::ImageViewType::e2D,
        surfaceFormat.format,
        componentMapping,
        subResourceRange);

    Buffers_.emplace_back(SwapChainBuffer{image, {logicalDevice.GetDevice().createImageView(imageViewCreateInfo)}});
  }

}
vk::SurfaceFormatKHR GlassEye::VulkanSwapChain::ChooseSwapChainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &AvailableFormats) {
  for (const auto &availableFormat : AvailableFormats) {
    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return availableFormat;
    }
  }
  return AvailableFormats[0];
}
vk::PresentModeKHR GlassEye::VulkanSwapChain::ChooseSwapChainPresentMode(const std::vector<vk::PresentModeKHR> &AvailableFormats) {
  for (const auto &availablePresentMode : AvailableFormats) {
    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
      return availablePresentMode;
    }
  }

  return vk::PresentModeKHR::eFifo;
}
GlassEye::VulkanSwapChain::~VulkanSwapChain() {
  if (SwapChain_) {
    for (uint32 i = 0; i < Buffers_.size(); i++) {
      LogicalDevice_->GetDevice().destroy(Buffers_[i].view);
    }
  }

  LogicalDevice_->GetDevice().destroy(SwapChain_);

}
const std::vector<GlassEye::VulkanSwapChain::SwapChainBuffer> &GlassEye::VulkanSwapChain::GetSwapChainBuffers() const {
  return Buffers_;
}
const vk::Format &GlassEye::VulkanSwapChain::GetFormat() const {
  return ImageFormat_;
}
const vk::PresentModeKHR GlassEye::VulkanSwapChain::GetPresentMode() const {
  return PresentMode_;
}
const vk::Extent2D GlassEye::VulkanSwapChain::GetExtent() const {
  return Extent_;
}
const vk::SwapchainKHR GlassEye::VulkanSwapChain::GetSwapChain() const {
  return SwapChain_;
}
GlassEye::uint32 GlassEye::VulkanSwapChain::GetViewCount() const {
  return (uint32) Buffers_.size();
}

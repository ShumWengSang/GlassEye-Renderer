//
// Created by user on 6/18/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanSwapChain.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/18/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSWAPCHAIN_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSWAPCHAIN_H_
namespace GlassEye {
class VulkanSwapChain {
 public:
  struct SwapChainBuffer {
    vk::Image image;
    vk::ImageView view;
  };
 public:
  VulkanSwapChain(
      const vk::Extent2D &extent,
      const class VulkanLogicalDevice &logicalDevice,
      const class VulkanPhysicalDevice &physicalDevice,
      const vk::SurfaceKHR &surface);

  ~VulkanSwapChain();
  const std::vector<SwapChainBuffer> &GetSwapChainBuffers() const;
  const vk::Format &GetFormat() const;
  const vk::PresentModeKHR GetPresentMode() const;
  const vk::Extent2D GetExtent() const;
  const vk::SwapchainKHR GetSwapChain() const;
  uint32 GetViewCount() const;
 private:
  /**
  * Choose a swap chain format based on the available formats. Prefer to format
  * that has VK_FORMAT_B8G8R8A8_UNORM and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, otherwise
  * get the first available.
  */
  static vk::SurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &AvailableFormats);

  /**
  * Choose a present mode for the swap chain based on the given formats. Prefer VK_PRESENT_MODE_MAILBOX_KHR
  * If none are available, than return VK_PRESENT_MODE_FIFO_KHR or VK_PRESENT_MODE_IMMEDIATE_KHR based on support
  */
  static vk::PresentModeKHR ChooseSwapChainPresentMode(const std::vector<vk::PresentModeKHR> &AvialableFormats);

 private:
  vk::SwapchainKHR SwapChain_{};
  vk::Extent2D Extent_{};
  vk::Format ImageFormat_{};
  vk::PresentModeKHR PresentMode_{};
  const class VulkanLogicalDevice *LogicalDevice_;

  std::vector<SwapChainBuffer> Buffers_;

  // Todo: Clean up sepeartion of swap chain and context
  friend class VulkanContext;
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANSWAPCHAIN_H_

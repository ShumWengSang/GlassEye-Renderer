//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanContext.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANCONTEXT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANCONTEXT_H_

namespace GlassEye {

class VulkanContext {
 public:
  VulkanContext() = default;
  ~VulkanContext() = default;

  void Create(GLFWwindow *window);
  void Destroy();
  void SwapBuffers();
  void OnResize(uint32 width, uint32 height);

  const std::vector<const char *> &GetEnabledExtensions() const;
  const std::vector<const char *> &GetEnabledLayers() const;

  const vk::CommandPool &GetPool() const;
  class VulkanPhysicalDevice *GetPhyiscalDevice() const;
  class VulkanLogicalDevice *GetLogicalDevice() const;
  class VulkanSwapChain *GetSwapChain() const;
  vk::Instance GetInstance() const;

  void BeginFrame();
  void EndFrame();
  void BeginRenderPass();
  void EndRenderPass();

  static const VulkanContext *GetContext();

  // Utility Commands
  vk::CommandBuffer BeginSingleTimeCommands() const;
  void EndSingleTimeCommands(vk::CommandBuffer command) const;
  void TransitionImageLayout(vk::Image image,
                             vk::Format format,
                             vk::ImageLayout oldLayout,
                             vk::ImageLayout newLayout,
                             uint32 mipLevels = 1) const;
 private:

  vk::Instance Instance_;
  vk::DebugUtilsMessengerEXT DebugCallback = VK_NULL_HANDLE;
  vk::SurfaceKHR Surface_;
  GLFWwindow *Window_ = nullptr;
  Unique <VulkanPhysicalDevice> PhysicalDevice_;
  Unique <VulkanLogicalDevice> LogicalDevice_;
  Unique <VulkanSwapChain> SwapChain_;
  std::vector<const char *> EnabledExtensions;
  std::vector<const char *> EnabledLayers;
  Unique<class VulkanDepthBuffer> DepthBuffer_;

  vk::CommandPool CommandPool_;
  std::vector<vk::CommandBuffer> DrawCommandBuffers_;

  std::vector<vk::Semaphore> PresentComplete_;
  std::vector<vk::Semaphore> RenderComplete_;
  std::vector<vk::Fence> InFlightFences_;

  std::vector<VkClearValue> ClearValues_ = std::vector<VkClearValue>(2);

  vk::RenderPass globalRenderPass_;
  std::vector<vk::Framebuffer> SwapChainFrameBuffers_;
  static inline VulkanContext *theOnlyContextThereShouldEverBe = nullptr;
 private:

  vk::DebugUtilsMessengerCreateInfoEXT SetupDebugMessageInfo();
  void SetupDebugMessages(vk::Instance instance);
  vk::Extent2D ChooseSwapExtent(vk::SurfaceCapabilitiesKHR const &capabilities) const;
  void CreateCommandPoolAndBufferAllocations();
  void CreateSynchObjects();
  void BuildSwapChainResources();
  void BuildGlobalRenderPass();
  void BuildSwapChainFrameBuffer();

};

}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANCONTEXT_H_

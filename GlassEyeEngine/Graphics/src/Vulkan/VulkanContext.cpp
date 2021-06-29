//
// Created by user on 6/16/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanContext.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/16/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanContext.h"
#include "Logger.h"
#include "Vulkan/VulkanUtility.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanDepthBuffer.h"

#ifdef GLASSEYE_DEBUG
const static bool enableValidationLayers = true;
#else
const static bool enableValidationLayers = false;
#endif
static PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
static PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

static VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                    void *pUserData) {
  UNREFERENCED_PARAMETER(pUserData);
  UNREFERENCED_PARAMETER(messageType);
  UNREFERENCED_PARAMETER(messageSeverity);

  const char *loaderErrorStr = "loaderAddLayerProperties";
  // If we can't find substring
  if (strstr(pCallbackData->pMessage, loaderErrorStr) == NULL) {
    // Message is important enough to show
    GLASS_CORE_WARN("VulkanDebugCallback:\n  Severity: {0}\n   Object Type: {1}\n  Message: {2}",
                    vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity )),
                    vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageType )),
                    pCallbackData->pMessage);

  }
  return VK_FALSE;
}

void GlassEye::VulkanContext::Create(GLFWwindow *window) {
  GLASS_CORE_INFO("Initializing Vulkan");
  Window_ = window;

  if (!glfwVulkanSupported()) {
    GLASS_CORE_ERROR("GLFW must support Vulkan!");
    GLASSEYE_BREAK();
  }

  //////////////////// Application information///////////////////////////
  vk::ApplicationInfo applicationInfo(
      "Glass Eye Sandbox", 1, "Glass Eye Engine", 1, VK_API_VERSION_1_2);

  ////////////////// Instance Extensions and Instance Layers (with validation)  ////////////
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> glfwExtensionsV(glfwExtensions,
                                            glfwExtensions + glfwExtensionCount);
  std::vector<const char *> LocalEnabledExtensions = VulkanUtility::GatherExtensions(glfwExtensionsV
#ifdef GLASSEYE_DEBUG
      , vk::enumerateInstanceExtensionProperties()
#endif
  );
  std::vector<const char *> LocalEnabledLayers = VulkanUtility::GatherLayers({}
#ifdef GLASSEYE_DEBUG
      , vk::enumerateInstanceLayerProperties()
#endif
  );

  // Show what we got, and what we enabled
  if (enableValidationLayers) {
    std::vector<vk::ExtensionProperties> allExtensions = vk::enumerateInstanceExtensionProperties();
    GLASS_CORE_TRACE("Vulkan Available Extensions");
    for (const auto &extension : allExtensions) {
      GLASS_CORE_TRACE("  {0}", extension.extensionName);
    }

    // Print out active extensions and layers
    GLASS_CORE_TRACE("Vulkan Enabled Extensions");
    for (const auto &extension : LocalEnabledExtensions) {
      GLASS_CORE_TRACE("  {0}", extension);
    }
    GLASS_CORE_TRACE("Vulkan Enabled Layers");
    for (const auto &layer : LocalEnabledLayers) {
      GLASS_CORE_TRACE("  {0}", layer);
    }
  }
  this->EnabledExtensions = LocalEnabledExtensions;
  this->EnabledLayers = LocalEnabledLayers;
  //////////////////////////// Instance Creation //////////////////////////////
  if (enableValidationLayers) {
    /* Generate with debug messenager attached */
    vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> instanceCreateInfo(
        {{}, &applicationInfo, EnabledLayers, EnabledExtensions},
        SetupDebugMessageInfo());
    Instance_ = vk::createInstance(instanceCreateInfo.get<vk::InstanceCreateInfo>());
  } else {
    vk::StructureChain<vk::InstanceCreateInfo> instanceCreateInfo(
        {{}, &applicationInfo, EnabledLayers, EnabledExtensions});
    Instance_ = vk::createInstance(instanceCreateInfo.get<vk::InstanceCreateInfo>());
  }

  if (enableValidationLayers) {
    SetupDebugMessages(Instance_);
  }

  ////////////////////////////////////// Create Surface ////////////////////////////////////////////
  VkSurfaceKHR Surface_;
  VkResult err = glfwCreateWindowSurface(static_cast<VkInstance>( Instance_ ), Window_, nullptr, &Surface_);
  if (err != VK_SUCCESS)
    throw std::runtime_error("Failed to create window!");
  Surface_ = vk::SurfaceKHR(Surface_);

  ////////////////////////////////// Choose Physical and Create Logical Devices //////////////////////
  PhysicalDevice_ = MakeUnique<VulkanPhysicalDevice>(*this, Surface_);

  // Setup enabled features
  vk::PhysicalDeviceFeatures enabledFeatures;
  enabledFeatures.samplerAnisotropy = true;

  LogicalDevice_ = MakeUnique<VulkanLogicalDevice>(*this, *PhysicalDevice_, enabledFeatures);

  ///////////////////////////////////// Create Swap Chain ////////////////////////////////////////////
  SwapChain_ = MakeUnique<VulkanSwapChain>(ChooseSwapExtent(PhysicalDevice_->GetSwapChainSupportDetails().capabilities),
                                           *LogicalDevice_,
                                           *PhysicalDevice_,
                                           Surface_);

  /////////////////////////////// Create Command Pool + Allocatte Command Buffers_ /////////////////
  CreateCommandPoolAndBufferAllocations();


  ///////////////////////////////////// Synchronization Objects //////////////////////////
  CreateSynchObjects();

  ///////////////////////////////////// Swap Chain Resources /////////////////////////////
  BuildSwapChainResources();

  theOnlyContextThereShouldEverBe = this;

}
void GlassEye::VulkanContext::SwapBuffers() {

}
void GlassEye::VulkanContext::OnResize(GlassEye::uint32 width, GlassEye::uint32 height) {

}
void GlassEye::VulkanContext::BeginFrame() {

}

void GlassEye::VulkanContext::SetupDebugMessages(vk::Instance instance) {
  /* Load the creation and destruction debug message functions */
  pfnVkCreateDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>( instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
  if (!pfnVkCreateDebugUtilsMessengerEXT) {
    GLASS_CORE_ERROR("GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function.");
    GLASSEYE_BREAK();
  }

  pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
  if (!pfnVkDestroyDebugUtilsMessengerEXT) {
    GLASS_CORE_ERROR("GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function.");
    GLASSEYE_BREAK();
  }


  // instanceCreateInfo
  DebugCallback = instance.createDebugUtilsMessengerEXT(SetupDebugMessageInfo());

}

vk::DebugUtilsMessengerCreateInfoEXT GlassEye::VulkanContext::SetupDebugMessageInfo() {
  vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
  return vk::DebugUtilsMessengerCreateInfoEXT(
      {}, severityFlags, messageTypeFlags, &VulkanDebugCallback);
}
const std::vector<const char *> &GlassEye::VulkanContext::GetEnabledExtensions() const {
  return EnabledExtensions;
}
const std::vector<const char *> &GlassEye::VulkanContext::GetEnabledLayers() const {
  return EnabledLayers;
}
vk::Extent2D GlassEye::VulkanContext::ChooseSwapExtent(vk::SurfaceCapabilitiesKHR const &capabilities) const {
  if (capabilities.currentExtent.width != std::numeric_limits<UINT32>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(Window_, &width, &height);

    auto width2 = static_cast<uint32>(width);
    auto height2 = static_cast<uint32>(height);

    VkExtent2D actualExtent = {width2, height2};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
void GlassEye::VulkanContext::CreateCommandPoolAndBufferAllocations() {
  // Create one command buffer for each swap chain image and reuse for rendering
  DrawCommandBuffers_.resize(SwapChain_->GetViewCount());

  CommandPool_ = VulkanUtility::CreateCommandPool(
      LogicalDevice_->GetDevice(),
      PhysicalDevice_->GetQueueFamilyIndices().Graphics.value());

  vk::CommandBufferAllocateInfo
      commandBufferAllocate(CommandPool_, vk::CommandBufferLevel::ePrimary, (uint32) DrawCommandBuffers_.size());
  DrawCommandBuffers_ = LogicalDevice_->GetDevice().allocateCommandBuffers(commandBufferAllocate);
}
void GlassEye::VulkanContext::CreateSynchObjects() {
  PresentComplete_.resize(VulkanUtility::MAX_FRAMES_IN_FLIGHT);
  RenderComplete_.resize(VulkanUtility::MAX_FRAMES_IN_FLIGHT);
  InFlightFences_.resize(VulkanUtility::MAX_FRAMES_IN_FLIGHT);

  vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};
  vk::SemaphoreCreateInfo semaphoreInfo;

  const vk::Device &device = LogicalDevice_->GetDevice();

  for (size_t i = 0; i < VulkanUtility::MAX_FRAMES_IN_FLIGHT; i++) {
    PresentComplete_[i] = device.createSemaphore(semaphoreInfo);
    RenderComplete_[i] = device.createSemaphore(semaphoreInfo);
    InFlightFences_[i] = device.createFence(fenceInfo);
  }

}
const vk::CommandPool &GlassEye::VulkanContext::GetPool() const {
  return CommandPool_;
}
GlassEye::VulkanPhysicalDevice *GlassEye::VulkanContext::GetPhyiscalDevice() const {
  return PhysicalDevice_.get();
}
GlassEye::VulkanLogicalDevice *GlassEye::VulkanContext::GetLogicalDevice() const {
  return LogicalDevice_.get();
}
GlassEye::VulkanSwapChain *GlassEye::VulkanContext::GetSwapChain() const {
  return SwapChain_.get();
}
void GlassEye::VulkanContext::BuildSwapChainResources() {
  ClearValues_[0].color = {0.0f, 0.0f, 0.0f, 0.2F};
  ClearValues_[1].depthStencil = {1.0f, ~0U};

  DepthBuffer_ = MakeUnique<VulkanDepthBuffer>(*this,
                                               *PhysicalDevice_,
                                               *LogicalDevice_,
                                               vk::SampleCountFlagBits::e1,
                                               SwapChain_->GetExtent());
  BuildGlobalRenderPass();
  BuildSwapChainFrameBuffer();
}

void GlassEye::VulkanContext::BuildGlobalRenderPass() {
  vk::AttachmentDescription colorAttachment{
      {},
      SwapChain_->GetFormat(),
      vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear,
      vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::ePresentSrcKHR};

  vk::AttachmentDescription depthAttachment{
      {},
      DepthBuffer_->GetFormat(),
      vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear,
      vk::AttachmentStoreOp::eDontCare,
      vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eDepthStencilAttachmentOptimal
  };

  vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
  vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::SubpassDescription subpassDesc({},
                                     vk::PipelineBindPoint::eGraphics,
                                     {},
                                     colorAttachmentRef,
                                     {},
                                     &depthAttachmentRef);

  vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0,
                                   {vk::PipelineStageFlagBits::eColorAttachmentOutput
                                        | vk::PipelineStageFlagBits::eEarlyFragmentTests},
                                   vk::PipelineStageFlagBits::eColorAttachmentOutput
                                       | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                   {},
                                   {vk::AccessFlagBits::eColorAttachmentRead
                                        | vk::AccessFlagBits::eDepthStencilAttachmentWrite}
  );

  std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
  vk::RenderPassCreateInfo renderPassCreateInfo(vk::RenderPassCreateFlags(),
                                                attachments,
                                                subpassDesc,
                                                dependency
  );
  globalRenderPass_ = LogicalDevice_->GetDevice().createRenderPass(renderPassCreateInfo);
}
vk::CommandBuffer GlassEye::VulkanContext::BeginSingleTimeCommands() const {
  return VulkanUtility::BeginSingleTimeCommands(LogicalDevice_->GetDevice(), CommandPool_);
}
void GlassEye::VulkanContext::EndSingleTimeCommands(vk::CommandBuffer command) const {
  return VulkanUtility::EndSingleTimeCommands(command,
                                              LogicalDevice_->GetDevice(),
                                              CommandPool_,
                                              LogicalDevice_->GetGraphicsQueue());
}
void GlassEye::VulkanContext::TransitionImageLayout(vk::Image image,
                                                    vk::Format format,
                                                    vk::ImageLayout oldLayout,
                                                    vk::ImageLayout newLayout,
                                                    GlassEye::uint32 mipLevels) const {
  vk::CommandBuffer commandBuffer = BeginSingleTimeCommands();
  vk::AccessFlags sourceAccessMask{};
  vk::AccessFlags destAccessMask{};
  vk::PipelineStageFlags sourceStage{};
  vk::PipelineStageFlags destinationStage{};

  if (oldLayout == vk::ImageLayout::eUndefined &&
      newLayout == vk::ImageLayout::eTransferDstOptimal) {

    destAccessMask = vk::AccessFlagBits::eTransferWrite;

    sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
    destinationStage = vk::PipelineStageFlagBits::eTransfer;
  } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
      newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
    sourceAccessMask = vk::AccessFlagBits::eTransferWrite;
    destAccessMask = vk::AccessFlagBits::eShaderRead;

    sourceStage = vk::PipelineStageFlagBits::eTransfer;
    destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }

  vk::ImageMemoryBarrier barrier{
      sourceAccessMask, // todo
      destAccessMask,
      oldLayout,
      newLayout,
      VK_QUEUE_FAMILY_IGNORED,
      VK_QUEUE_FAMILY_IGNORED,
      image,
      vk::ImageSubresourceRange(
          {
              vk::ImageAspectFlagBits::eColor,
              0,
              mipLevels,
              0,
              1
          }
      )
  };
  commandBuffer.pipelineBarrier(sourceStage, destinationStage,
      /* Dependency flags*/{},
      /* Memory Barriers*/{},
      /* Buffer Memory Barrier*/{},
                                {barrier});
  EndSingleTimeCommands(commandBuffer);
}
void GlassEye::VulkanContext::BuildSwapChainFrameBuffer() {
  const std::vector<VulkanSwapChain::SwapChainBuffer> &SwapChainImages = SwapChain_->GetSwapChainBuffers();
  SwapChainFrameBuffers_.resize(SwapChainImages.size());
  for (size_t i = 0; i < SwapChainImages.size(); i++) {
    std::array<vk::ImageView, 2> attachments = {
        SwapChainImages[i].view,
        DepthBuffer_->GetImageView()
    };

    vk::FramebufferCreateInfo createInfo({}, globalRenderPass_, attachments, SwapChain_->GetExtent().width,
                                         SwapChain_->GetExtent().height, 1);

    SwapChainFrameBuffers_[i] = LogicalDevice_->GetDevice().createFramebuffer(createInfo, nullptr);
  }
}

void GlassEye::VulkanContext::Destroy() {
  const vk::Device &device = LogicalDevice_->GetDevice();
  device.waitIdle();

  for (auto &framebuffer : SwapChainFrameBuffers_) {
    device.destroy(framebuffer, nullptr);
  }

  device.destroy(globalRenderPass_);

  DepthBuffer_.reset();
  SwapChain_.reset();

  for (int i = 0; i < VulkanUtility::MAX_FRAMES_IN_FLIGHT; ++i) {
    device.destroy(PresentComplete_[i]);
    device.destroy(RenderComplete_[i]);
    device.destroy(InFlightFences_[i]);
  }

  device.destroy(CommandPool_);

  Instance_.destroySurfaceKHR(Surface_);

  if (enableValidationLayers) {
    Instance_.destroyDebugUtilsMessengerEXT(DebugCallback);
  }

  LogicalDevice_.reset();
  PhysicalDevice_.reset();

  Instance_.destroy();

  theOnlyContextThereShouldEverBe = nullptr;
}

const GlassEye::VulkanContext *GlassEye::VulkanContext::GetContext() {
  if (!theOnlyContextThereShouldEverBe)
    GLASS_CORE_WARN("Retreiving a null rendering context");
  return theOnlyContextThereShouldEverBe;
}
vk::Instance GlassEye::VulkanContext::GetInstance() const {
  return Instance_;
}


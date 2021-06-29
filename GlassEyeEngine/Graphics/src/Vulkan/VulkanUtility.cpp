//
// Created by user on 6/17/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanUtility.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/17/2021
 * End Header --------------------------------------------------------*/
#include <ObjIdlbase.h>
#include "EnginePCH.h"
#include "Vulkan/VulkanUtility.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"

std::vector<char const *> GlassEye::VulkanUtility::GatherLayers(const std::vector<const char *> &layers
#ifdef GLASSEYE_DEBUG
    , const std::vector<vk::LayerProperties> &layerProperties
#endif
) {
  std::vector<char const *> enabledLayers;
  enabledLayers.reserve(layers.size());
  for (auto const &layer : layers) {
    assert(std::find_if(layerProperties.begin(),
                        layerProperties.end(),
                        [layer](vk::LayerProperties const &lp) { return std::strcmp(layer, lp.layerName) == 0; })
               != layerProperties.end());
    enabledLayers.emplace_back(layer);
  }

  // Enable standard validation layer
#ifdef GLASSEYE_DEBUG
  {
    // If cannot find the khronos validation already in, then find and add in
    if (std::find(layers.begin(), layers.end(), "VK_LAYER_KHRONOS_validation") == layers.end() &&
        std::find_if(layerProperties.begin(),
                     layerProperties.end(),
                     [](vk::LayerProperties const &lp) {
                       return (strcmp("VK_LAYER_KHRONOS_validation", lp.layerName) == 0);
                     }) != layerProperties.end()) {
      enabledLayers.emplace_back("VK_LAYER_KHRONOS_validation");
    }
  }
#endif
  return enabledLayers;
}
std::vector<char const *> GlassEye::VulkanUtility::GatherExtensions(const std::vector<const char *> &extensions
#ifdef GLASSEYE_DEBUG
    , const std::vector<vk::ExtensionProperties> &extensionProperties
#endif
) {
  std::vector<char const *> enabledExtensions;
  enabledExtensions.reserve(extensions.size());
  for (auto const &ext : extensions) {
    assert(std::find_if(extensionProperties.begin(),
                        extensionProperties.end(),
                        [ext](vk::ExtensionProperties const &ep) { return std::strcmp(ext, ep.extensionName) == 0; })
               != extensionProperties.end());
    enabledExtensions.emplace_back(ext);
  }
#ifdef GLASSEYE_DEBUG
  if (std::find(extensions.begin(), extensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == extensions.end() &&
      std::find_if(extensionProperties.begin(),
                   extensionProperties.end(),
                   [](vk::ExtensionProperties const &ep) {
                     return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0);
                   }) != extensionProperties.end()) {
    enabledExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
#endif
  return enabledExtensions;
}

std::vector<const char *> GlassEye::VulkanUtility::GetDeviceExtensions() {
  return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}

bool GlassEye::VulkanUtility::QueueFamilyIndices::isComplete() const {
  return Graphics.has_value() && Compute.has_value()
      && Transfer.has_value() && Present.has_value();
}

GlassEye::VulkanUtility::QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice &device,
                                                                const vk::SurfaceKHR &surface) {

  // get the QueueFamilyProperties of the first PhysicalDevice
  std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.getQueueFamilyProperties();

  for (uint32 i = 0; i < queueFamilyProperties.size(); ++i) {
    // Check for graphics support.
    if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
      Graphics = i;
    }

    // Check for presentation support.
    if (device.getSurfaceSupportKHR(i, surface)) {
      Present = i;
    }

    // Check for compute support.
    if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute) {
      Compute = i;
    }

    // Check for transfer support.
    if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer) {
      Transfer = i;
    }

    if (isComplete()) {
      break;
    }
  }

  if (!Graphics.has_value()) {
    GLASS_CORE_CRITICAL("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT");
    GLASSEYE_BREAK();
  }
}

std::pair<GlassEye::uint32, GlassEye::uint32> GlassEye::VulkanUtility::QueueFamilyIndices::FindGraphicsAndPresent(
    vk::PhysicalDevice physicalDevice,
    vk::SurfaceKHR const &surface) {
  std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
  assert(queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max());

  uint32 graphicsQueueFamilyIndex = FindGraphicsQueueFamilyIndex(queueFamilyProperties);
  if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, surface)) {
    return std::make_pair(
        graphicsQueueFamilyIndex,
        graphicsQueueFamilyIndex);  // the first graphicsQueueFamilyIndex does also support presents
  }

  // the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both
  // graphics and present
  for (size_t i = 0; i < queueFamilyProperties.size(); i++) {
    if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
        physicalDevice.getSurfaceSupportKHR(static_cast<uint32>( i ), surface)) {
      return std::make_pair(static_cast<uint32>( i ), static_cast<uint32>( i ));
    }
  }

  // there's nothing like a single family index that supports both graphics and present -> look for an other family
  // index that supports present
  for (size_t i = 0; i < queueFamilyProperties.size(); i++) {
    if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32>( i ), surface)) {
      return std::make_pair(graphicsQueueFamilyIndex, static_cast<uint32>( i ));
    }
  }

  throw std::runtime_error("Could not find queues for both graphics or present -> terminating");
}

GlassEye::uint32 GlassEye::VulkanUtility::QueueFamilyIndices::FindGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const &queueFamilyProperties) {
  // get the first index into queueFamiliyProperties which supports graphics
  auto graphicsQueueFamilyProperty = std::find_if(
      queueFamilyProperties.begin(),
      queueFamilyProperties.end(),
      [](vk::QueueFamilyProperties const &qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; });
  assert(graphicsQueueFamilyProperty != queueFamilyProperties.end());
  return static_cast<uint32_t>( std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));
}

vk::CommandPool GlassEye::VulkanUtility::CreateCommandPool(const vk::Device &device,
                                                           uint32_t queueFamilyIndex) {
  vk::CommandPoolCreateInfo commandPoolCreateInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                                  queueFamilyIndex);
  return device.createCommandPool(commandPoolCreateInfo);
}
vk::CommandBuffer GlassEye::VulkanUtility::BeginSingleTimeCommands(const vk::Device &device,
                                                                   const vk::CommandPool &pool) {
  vk::CommandBufferAllocateInfo allocInfo({pool, vk::CommandBufferLevel::ePrimary, 1});
  vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(allocInfo).front();
  vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

  commandBuffer.begin(beginInfo);

  return commandBuffer;
}
void GlassEye::VulkanUtility::EndSingleTimeCommands(vk::CommandBuffer commandBuffer,
                                                    const vk::Device &device,
                                                    const vk::CommandPool &pool,
                                                    const vk::Queue &queue) {
  commandBuffer.end();

  vk::SubmitInfo submiInfo{};
  submiInfo.commandBufferCount = 1;
  submiInfo.pCommandBuffers = &commandBuffer;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"
  queue.submit(1, &submiInfo, nullptr);
#pragma clang diagnostic pop
  queue.waitIdle();

  device.freeCommandBuffers(pool, 1, &commandBuffer);
}

GlassEye::VulkanUtility::SwapChainSupportDetails::SwapChainSupportDetails(const vk::PhysicalDevice &device,
                                                                          const vk::SurfaceKHR &surface) {
  this->capabilities = device.getSurfaceCapabilitiesKHR(surface);

  // 2) Get formats of this surface
  this->formats = device.getSurfaceFormatsKHR(surface);

  // 3) Get available presentation modes
  this->presentModes = device.getSurfacePresentModesKHR(surface);
}

vk::Format GlassEye::VulkanUtility::FindSupportedFormat(const std::vector<vk::Format> &candidates,
                                                        vk::ImageTiling tiling,
                                                        vk::FormatFeatureFlags features,
                                                        vk::PhysicalDevice device) {
  for (vk::Format format : candidates) {
    vk::FormatProperties props = device.getFormatProperties(format);
    if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }
  throw std::runtime_error("failed to find supported format!");
}
uint32_t GlassEye::VulkanUtility::FindMemoryType(GlassEye::uint32 typeFilter,
                                                 vk::MemoryPropertyFlags properties,
                                                 vk::PhysicalDevice physicalDevice) {
  vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

  for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
    if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags &
        properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("failed to find suitable memory type!");
}
void GlassEye::VulkanUtility::CreateImage(vk::Device device,
                                          vk::PhysicalDevice physicalDevice,
                                          GlassEye::uint32 width,
                                          GlassEye::uint32 height,
                                          GlassEye::uint32 mipmapLevel,
                                          GlassEye::uint32 depth,
                                          GlassEye::uint32 arrayLayers,
                                          vk::Format format,
                                          vk::ImageTiling tiling,
                                          vk::ImageUsageFlags usage,
                                          vk::MemoryPropertyFlags memPropertyFlags,
                                          vk::ImageCreateFlags imageCreateFlags,
                                          vk::Image &image,
                                          vk::DeviceMemory &memory,
                                          vk::SampleCountFlagBits numSamples) {
  vk::Extent3D extent{width, height, depth};
  vk::ImageCreateInfo imageCreateInfo(
      {},
      vk::ImageType::e2D,
      format,
      extent,
      mipmapLevel,
      arrayLayers,
      numSamples,
      tiling,
      usage,
      vk::SharingMode::eExclusive,
      {}
  );
  image = device.createImage(imageCreateInfo);

  vk::MemoryRequirements MemoryReq = device.getImageMemoryRequirements(image);
  vk::MemoryAllocateInfo allocInfo{
      MemoryReq.size,
      VulkanUtility::FindMemoryType(MemoryReq.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, physicalDevice)
  };
  memory = device.allocateMemory(allocInfo);
  device.bindImageMemory(image, memory, 0);
}
void GlassEye::VulkanUtility::CreateImage(vk::Device device,
                                          vk::PhysicalDevice physicalDevice,
                                          GlassEye::uint32 width,
                                          GlassEye::uint32 height,
                                          vk::Format format,
                                          vk::ImageTiling tiling,
                                          vk::ImageUsageFlags usage,
                                          vk::MemoryPropertyFlags memPropertyFlags,
                                          vk::Image &image,
                                          vk::DeviceMemory &memory,
                                          vk::SampleCountFlagBits numSamples) {
  CreateImage(
      device,
      physicalDevice,
      width,
      height,
      /* t_MipLevels */ 1,
      /* t_Depth */ 1,
      /* t_ArrayLayers */ 1,
      format,
      tiling,
      usage,
      memPropertyFlags,
      /* t_flags */ {},
      image,
      memory,
      numSamples
  );
}
bool GlassEye::VulkanUtility::HasStencilComponent(vk::Format format) {
  return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}
vk::ImageView GlassEye::VulkanUtility::CreateImageView(vk::Device device,
                                                       vk::Image image,
                                                       vk::Format format,
                                                       vk::ImageAspectFlags aspectFlags,
                                                       GlassEye::uint32 mipLevels) {
  vk::ImageViewCreateInfo viewInfo(
      {},
      image,
      vk::ImageViewType::e2D,
      format,
      {},
      {
          aspectFlags,
          0,
          mipLevels,
          0,
          1
      }
  );
  vk::ImageView imageView = device.createImageView(viewInfo, nullptr);

  return imageView;
}

//
// Created by user on 6/17/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanUtility.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/17/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANUTILITY_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANUTILITY_H_
namespace GlassEye {
namespace VulkanUtility {

const int MAX_FRAMES_IN_FLIGHT = 2;

// Eye gouging code that actually makes sense
std::vector<char const *> GatherLayers(std::vector<const char *> const &layers
#ifdef GLASSEYE_DEBUG
    , std::vector<vk::LayerProperties> const &layerProperties
#endif
);

std::vector<char const *> GatherExtensions(std::vector<const char *> const &extensions
#ifdef GLASSEYE_DEBUG
    , std::vector<vk::ExtensionProperties> const &extensionProperties
#endif
);

std::vector<const char *> GetDeviceExtensions();

struct SwapChainSupportDetails {
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> presentModes;

  SwapChainSupportDetails(const vk::PhysicalDevice &device,
                          const vk::SurfaceKHR &surface);
  SwapChainSupportDetails() = default;
};

struct QueueFamilyIndices {
  std::optional<uint32> Graphics;
  std::optional<uint32> Compute;
  std::optional<uint32> Transfer;
  std::optional<uint32> Present;
  bool isComplete() const;

  QueueFamilyIndices(const vk::PhysicalDevice &device,
                     const vk::SurfaceKHR &surface);
  QueueFamilyIndices() = default;

  // Utility functions from https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/samples/utils/utils.cpp
  static std::pair<uint32, uint32> FindGraphicsAndPresent(vk::PhysicalDevice physicalDevice,
                                                          vk::SurfaceKHR const &surface);
  static uint32 FindGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const &queueFamilyProperties);

};

vk::CommandPool CreateCommandPool(vk::Device const &device, uint32_t queueFamilyIndex);

vk::CommandBuffer BeginSingleTimeCommands(vk::Device const &device, vk::CommandPool const &pool);

void EndSingleTimeCommands(vk::CommandBuffer commandBuffer, vk::Device const &device,
                           vk::CommandPool const &pool, vk::Queue const &queue);

vk::Format FindSupportedFormat(const std::vector<vk::Format> &candidates,
                               vk::ImageTiling tiling,
                               vk::FormatFeatureFlags features,
                               vk::PhysicalDevice device);
uint32_t FindMemoryType(uint32 typeFilter,
                        vk::MemoryPropertyFlags properties,
                        vk::PhysicalDevice physicalDevice);

void CreateImage(
    vk::Device device,
    vk::PhysicalDevice physicalDevice,
    uint32 width,
    uint32 height,
    vk::Format format,
    vk::ImageTiling tiling,
    vk::ImageUsageFlags usage,
    vk::MemoryPropertyFlags memPropertyFlags,
    vk::Image &image,
    vk::DeviceMemory &memory,
    vk::SampleCountFlagBits numSamples);

void CreateImage(vk::Device device,
                 vk::PhysicalDevice physicalDevice,
                 uint32 width,
                 uint32 height,
                 uint32 mipmapLevel,
                 uint32 depth,
                 uint32 arrayLayers,
                 vk::Format format,
                 vk::ImageTiling tiling,
                 vk::ImageUsageFlags usage,
                 vk::MemoryPropertyFlags memPropertyFlags,
                 vk::ImageCreateFlags imageCreateFlags,
                 vk::Image &image,
                 vk::DeviceMemory &memory,
                 vk::SampleCountFlagBits numSamples);

vk::ImageView CreateImageView(vk::Device device,
                              vk::Image image,
                              vk::Format format,
                              vk::ImageAspectFlags aspectFlags,
                              uint32 mipLevels = 1);

bool HasStencilComponent(vk::Format format);

}
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_VULKAN_VULKANUTILITY_H_

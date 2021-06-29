//
// Created by user on 6/17/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: VulkanPhysicalDevice.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/17/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanUtility.h"
#include "Logger.h"

// Local Functions
namespace {
// Taken from https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/samples/PhysicalDeviceProperties/PhysicalDeviceProperties.cpp
std::string decodeAPIVersion(uint32_t apiVersion) {
  return std::to_string(VK_VERSION_MAJOR(apiVersion)) + "." + std::to_string(VK_VERSION_MINOR(apiVersion)) +
      "." + std::to_string(VK_VERSION_PATCH(apiVersion));
}

std::string decodeDriverVersion(uint32_t driverVersion, uint32_t vendorID) {
  switch (vendorID) {
    case 4318:
      return std::to_string((driverVersion >> 22) & 0x3FF) + "." +
          std::to_string((driverVersion >> 14) & 0xFF) + "." + std::to_string((driverVersion >> 6) & 0xFF) +
          "." + std::to_string(driverVersion & 0x3F);
    case 0x8086:return std::to_string((driverVersion >> 14) & 0x3FFFF) + "." + std::to_string((driverVersion & 0x3FFF));
    default: return decodeAPIVersion(driverVersion);
  }
}

std::string decodeVendorID(uint32_t vendorID) {
  // below 0x10000 are the PCI vendor IDs (https://pcisig.com/membership/member-companies)
  if (vendorID < 0x10000) {
    switch (vendorID) {
      case 0x1022: return "Advanced Micro Devices";
      case 0x10DE: return "NVidia Corporation";
      case 0x8086: return "Intel Corporation";
      default: return std::to_string(vendorID);
    }
  } else {
    // above 0x10000 should be vkVendorIDs
    return vk::to_string(vk::VendorId(vendorID));
  }
}
}

GlassEye::VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanContext &context, const vk::SurfaceKHR &surface) {
  vk::Instance instance = context.GetInstance();

  // Get all physical devices
  std::vector<vk::PhysicalDevice> AllPhysicalDevices = instance.enumeratePhysicalDevices();

  if (AllPhysicalDevices.size() <= 0) {
    GLASS_CORE_ERROR("Failed to find GPU with Vulkan support!");
    GLASSEYE_BREAK();
  }

  // Choose best fit physical device based on requirements
  PhysicalDevice_ = ChooseBestPhysicalDevice(AllPhysicalDevices, surface);
  if (!PhysicalDevice_) {
    GLASS_CORE_CRITICAL("Was not able to find a Physical GPU that matches all requirements");
    GLASSEYE_BREAK();
  }

  // Now we log information regarding this physical device
  DeviceProperties_ = PhysicalDevice_.getProperties();
  DeviceFeatures_ = PhysicalDevice_.getFeatures();
  MemoryProperties_ = PhysicalDevice_.getMemoryProperties();
  MsaaSamples_ = GetMaxUsableSampleCount(PhysicalDevice_);

  // Todo: Extra call here to find queue indices maybe removed and use result from searching for physical driver
  auto[Graphics, Present] = VulkanUtility::QueueFamilyIndices::FindGraphicsAndPresent(PhysicalDevice_, surface);
  QueueFamilyIndices_.Graphics = Graphics;
  QueueFamilyIndices_.Present = Present;
  // Todo: This too
  SwapChainSupportDetails_ = VulkanUtility::SwapChainSupportDetails(PhysicalDevice_, surface);

  // Log to console
  LogPhysicalDevice();
}

vk::PhysicalDevice GlassEye::VulkanPhysicalDevice::ChooseBestPhysicalDevice(
    const std::vector<vk::PhysicalDevice> &AvailableDevices, const vk::SurfaceKHR &surface) const {
  // Todo: Use a scoring system
  // For now we find the first gpu that can support all the features we want.

  for (const auto &physicalDevice : AvailableDevices) {
    // Device supports the queue we want
    VulkanUtility::QueueFamilyIndices indices;

    // Todo: Maybe have to remove try catch
    try {
      auto[Graphics, Present] = VulkanUtility::QueueFamilyIndices::FindGraphicsAndPresent(physicalDevice, surface);
      indices.Graphics = Graphics;
      indices.Present = Present;
    }
    catch (const std::exception &exception) {
      UNREFERENCED_PARAMETER(exception);
      // Device does not have a queue for presenting and graphics
      continue;
    }

    // Device supports the extensions we want (swap chain)
    bool deviceSupportsExtension = CheckDeviceExtensionSupport(physicalDevice);

    // Device swap chain support is compatible with surface
    bool swapChainCompatible = false;
    if (deviceSupportsExtension) {
      VulkanUtility::SwapChainSupportDetails details(physicalDevice, surface);
      swapChainCompatible = !details.presentModes.empty() && !details.formats.empty();
    }

    // Check if physical device has all the features we need.
    bool physicalDeviceHasFeatures = CheckDeviceFeatures(physicalDevice);

    // If all of these requirements are met, we can use the physical device.
    if (indices.isComplete() && deviceSupportsExtension && swapChainCompatible && physicalDeviceHasFeatures) {
      return physicalDevice;
    }
  }
  return nullptr;
}

bool GlassEye::VulkanPhysicalDevice::CheckDeviceExtensionSupport(vk::PhysicalDevice const &device) const {
  // Get the extension support of this device
  std::vector<vk::ExtensionProperties> ExtensionProperties = device.enumerateDeviceExtensionProperties();
  auto deviceExtensions = VulkanUtility::GetDeviceExtensions();
  std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

  for (const auto &extension : ExtensionProperties) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

bool GlassEye::VulkanPhysicalDevice::CheckDeviceFeatures(vk::PhysicalDevice const &device) const {
  vk::PhysicalDeviceFeatures features = device.getFeatures();
  return features.samplerAnisotropy;
}

vk::SampleCountFlagBits GlassEye::VulkanPhysicalDevice::GetMaxUsableSampleCount(const vk::PhysicalDevice &device) const {
  vk::PhysicalDeviceProperties physicalDeviceProperties = device.getProperties();
  auto counts = physicalDeviceProperties.limits.framebufferColorSampleCounts
      & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

  if (counts & vk::SampleCountFlagBits::e64) {
    return vk::SampleCountFlagBits::e64;
  }
  if (counts & vk::SampleCountFlagBits::e32) {
    return vk::SampleCountFlagBits::e32;
  }
  if (counts & vk::SampleCountFlagBits::e16) {
    return vk::SampleCountFlagBits::e16;
  }
  if (counts & vk::SampleCountFlagBits::e8) {
    return vk::SampleCountFlagBits::e8;
  }
  if (counts & vk::SampleCountFlagBits::e4) {
    return vk::SampleCountFlagBits::e4;
  }
  if (counts & vk::SampleCountFlagBits::e2) {
    return vk::SampleCountFlagBits::e2;
  }

  return vk::SampleCountFlagBits::e1;
}
void GlassEye::VulkanPhysicalDevice::LogPhysicalDevice() {
  // Checks if the requested extensions are supported.
  std::vector<vk::ExtensionProperties> extensionProperties = PhysicalDevice_.enumerateDeviceExtensionProperties();
  std::stringstream DeviceInfo;

  DeviceInfo << "\nPhysical Device Info: \n\tType: " << vk::to_string(DeviceProperties_.deviceType);

  // ID
  DeviceInfo << "\n\tID: " << DeviceProperties_.deviceID;

  // Vendor
  DeviceInfo << "\n\tVendor:" << decodeVendorID(DeviceProperties_.vendorID);

  // Driver Version
  DeviceInfo << "\n\tDriver Version: "
             << decodeDriverVersion(DeviceProperties_.driverVersion, DeviceProperties_.vendorID);

  // API Version
  DeviceInfo << "\n\tAPI Version: " << decodeAPIVersion(DeviceProperties_.apiVersion);

  GLASS_CORE_TRACE("{0}\n", DeviceInfo.str());
}
const vk::PhysicalDevice &GlassEye::VulkanPhysicalDevice::GetPhysicalDevice() const {
  return PhysicalDevice_;
}
const GlassEye::VulkanUtility::QueueFamilyIndices &GlassEye::VulkanPhysicalDevice::GetQueueFamilyIndices() const {
  return QueueFamilyIndices_;
}
const GlassEye::VulkanUtility::SwapChainSupportDetails &GlassEye::VulkanPhysicalDevice::GetSwapChainSupportDetails() const {
  return SwapChainSupportDetails_;
}
vk::SampleCountFlagBits GlassEye::VulkanPhysicalDevice::GetMsaaSamples() const {
  return MsaaSamples_;
}

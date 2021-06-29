//
// Created by user on 6/21/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: AssetManager.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/21/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETMANAGER_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETMANAGER_H_
#include "Singleton.h"
#include "AssetImporter.h"
#include "AssetMetadata.h"
#include "FileSystem.h"
#include "AssetRegistry.h"

namespace GlassEye {
////////////////////////////////////// DECLARATIONS ////////////////////////////////////////////
class AssetManager : public Singleton<AssetManager> {
 public:
  using AssetsChangeEventFn = std::function<void(FileSystemChangedEvent)>;

 public:
  void Init() override;
  void Shutdown() override;
  void SetAssetsChangeEventFn(const AssetsChangeEventFn &callback);

  AssetMetadata &GetMetadata(AssetHandle handle);
  AssetMetadata &GetMetadata(std::string_view filepath);

  std::string GetFileSystemPath(const AssetMetadata &metadata);
  std::string GetRelativePath(std::string_view filepath);

  AssetHandle GetAssetHandleFromFilePath(std::string_view filepath);
  bool IsAssetHandleValid(AssetHandle assetHandle) { return GetMetadata(assetHandle).IsValid(); }

  AssetType GetAssetTypeFromExtension(std::string_view extension);
  AssetType GetAssetTypeFromPath(const std::filesystem::path &path);

  AssetHandle ImportAsset(std::string_view filepath);
  bool ReloadData(AssetHandle assetHandle);

  bool FileExists(AssetMetadata &metadata);

  template<typename T, typename ... Args>
  Ref <T> CreateNewAsset(std::string fileName, AssetHandle directoryHandle, Args &&... args);
  template<typename T>
  Ref <T> GetAsset(AssetHandle assetHandle);
  template<typename T>
  Ref <T> GetAsset(std::string_view filePath);

 private:
  void LoadAssetRegistry();
  void ProcessDirectory(const std::string &directoryPath);
  void ReloadAssets();
  void WriteRegistryToFile();

  static void OnFileSystemChanged(FileSystemChangedEvent e);
  void OnAssetRenamed(AssetHandle assetHandle, const std::string &newFilePath);
  void OnAssetMoved(AssetHandle assetHandle, const std::string &destinationPath);
  void OnAssetDeleted(AssetHandle assetHandle);

  bool AssetRegistryContains(const std::filesystem::path &filepath) const;
 private:
  using GuidToAssetMapType = std::unordered_map<AssetHandle, Ref < Asset>>;

  std::unordered_map<std::filesystem::path, AssetMetadata> AssetRegistry;
  GuidToAssetMapType LoadedAssets;
  AssetsChangeEventFn AssetsChangeCallback;

};

}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETMANAGER_H_

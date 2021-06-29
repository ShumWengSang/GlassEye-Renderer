//
// Created by user on 6/27/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_SRC_ASSETMANAGER_INL_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_SRC_ASSETMANAGER_INL_
#include "EnginePCH.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Utils.h"

namespace GlassEye {
template<typename T, typename... Args>
Ref<T> AssetManager::CreateNewAsset(std::string fileName,
                                    AssetHandle directoryHandle,
                                    Args &&... args) {
  static_assert(std::is_base_of<Asset, T>::value, "T must be derived from Asset!");
  auto directory = GetAsset<Directory>(directoryHandle);
  Ref<T> asset = MakeRef<T>(entt::hashed_string{fileName.c_str()}, std::forward<Args>(args)...);

  AssetMetadata metadata;
  metadata.Handle = asset->Handle;
  metadata.FilePath = asset->FilePath;
  metadata.Type = asset->Type;
  AssetRegistry[asset->FilePath] = metadata;

  asset->Handle_ = metadata.Handle;
  LoadedAssets[asset->Handle] = asset;
  AssetImporter::Serialize(asset);

  return asset;
}
template<typename T>
Ref<T> AssetManager::GetAsset(AssetHandle assetHandle) {
  if (LoadedAssets.template find(assetHandle) == LoadedAssets.end()) {
    GLASS_CORE_ERROR("Asset not found!");
    GLASSEYE_BREAK();
  }
  auto &metadata = GetMetadata(assetHandle);
  Ref<Asset> asset = nullptr;
  if (metadata.IsDataLoaded) {
    // Load the data
    metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
    if (!metadata.IsDataLoaded)
      return nullptr;

    LoadedAssets[assetHandle] = asset;
  } else {
    asset = LoadedAssets[assetHandle];
  }
  return std::dynamic_pointer_cast<T>(asset);
}
template<typename T>
Ref<T> AssetManager::GetAsset(std::string_view filePath) {

  return GetAsset < T > (this->GetAssetHandleFromFilePath(filePath));
}

}
#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_SRC_ASSETMANAGER_INL_

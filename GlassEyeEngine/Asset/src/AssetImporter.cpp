//
// Created by user on 6/26/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: AssetImporter.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/26/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "AssetImporter.h"
#include "Asset.h"
#include "AssetSerializer.h"
#include "AssetMetadata.h"
#include "AssetManager.h"

namespace GlassEye {
std::unordered_map<AssetType, Unique < AssetSerializer>>
Serializers;

void AssetImporter::Init() {
  Serializers[AssetType::SpirVShader] = MakeUnique<SpirVShaderSerializer>();
  Serializers[AssetType::Texture] = MakeUnique<TextureSerializer>();

}
void AssetImporter::Serialize(const Ref <Asset> &asset) {
  const AssetMetadata &metadata = AssetManager::Get().GetMetadata(asset->GetGuID());
  Serialize(metadata, asset);
}
void AssetImporter::Serialize(const AssetMetadata &metadata, const Ref <Asset> &asset) {
  if (Serializers.find(metadata.Type) == Serializers.end()) {
    GLASS_CORE_ERROR("There's currently no importer for assets of type {0}", metadata.FilePath.extension().string());
    return;
  }

  return Serializers[metadata.Type]->Serialize(metadata, asset);
}
bool AssetImporter::TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset) {
  if (Serializers.find(metadata.Type) == Serializers.end()) {
    GLASS_CORE_ERROR("There's currently no importer for assets of type {0}", metadata.FilePath.extension().string());
    return false;
  }

  return Serializers[metadata.Type]->TryLoadData(metadata, asset);
}
}
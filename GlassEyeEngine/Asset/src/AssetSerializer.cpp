//
// Created by user on 6/27/2021.
//
#include "EnginePCH.h"
#include "AssetSerializer.h"
#include "AssetMetadata.h"
#include "Asset.h"

bool GlassEye::TextureSerializer::TryLoadData(const AssetMetadata &metadata, Ref <GlassEye::Asset> &asset) const {
  return false;
}
bool GlassEye::SpirVShaderSerializer::TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset) const {
  return false;
}

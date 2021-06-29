//
// Created by user on 6/21/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Asset.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/21/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSET_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSET_H_
#include "AssetType.h"
namespace GlassEye {

enum class AssetFlag : uint16 {
  None = 0,
  Missing = BIT(0),
  Invalid = BIT(1)
};

// Asset represents a loaded asset in the engine
class Asset {
 public:
  explicit Asset(GUID id);
  uint16 Flags = (uint16) AssetFlag::None;

  virtual bool operator==(const Asset &other) const;

  virtual bool operator!=(const Asset &other) const;

  virtual ~Asset() = default;

  virtual AssetType GetAssetType() const { return AssetType::None; }

  bool IsFlagSet(AssetFlag flag) const;
  void SetFlag(AssetFlag flag, bool value);

  GUID GetGuID() const;
  std::string_view GetGuidString() const;
 protected:
  GUID Handle_;
  std::string GuidName;
};

class Directory : public Asset {
 public:
  Directory(GUID id);
  std::vector<AssetHandle> ChildDirectories;

  Directory() = default;
  static AssetType GetStaticType() { return AssetType::Directory; }
  AssetType GetAssetType() const override { return GetStaticType(); }
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSET_H_

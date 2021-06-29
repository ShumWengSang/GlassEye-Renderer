//
// Created by user on 6/21/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Asset.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/21/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "Asset.h"

bool GlassEye::Asset::operator!=(const GlassEye::Asset &other) const {
  return !(*this == other);
}
bool GlassEye::Asset::operator==(const GlassEye::Asset &other) const {
  // There is a chance of collision since its hashes
  return Handle_ == other.Handle_;
}
bool GlassEye::Asset::IsFlagSet(GlassEye::AssetFlag flag) const { return (uint16) flag & Flags; }
void GlassEye::Asset::SetFlag(GlassEye::AssetFlag flag, bool value) {
  if (value)
    Flags |= (uint16) flag;
  else
    Flags &= ~(uint16) flag;
}
std::string_view GlassEye::Asset::GetGuidString() const { return GuidName; }
GlassEye::GUID GlassEye::Asset::GetGuID() const { return Handle_; }
GlassEye::Asset::Asset(GlassEye::GUID id) : Handle_(id), GuidName(id.data()) {}
GlassEye::Directory::Directory(GlassEye::GUID id) : Asset(id) {}

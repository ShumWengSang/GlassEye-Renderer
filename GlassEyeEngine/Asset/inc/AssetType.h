//
// Created by user on 6/28/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETTYPE_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETTYPE_H_
namespace GlassEye {
enum class AssetType : uint8 {
  SpirVShader,
  Texture,
  Directory,
  Mesh,
  Other,
  None,
  Missing
};

inline static std::unordered_map<std::string, AssetType> AssetExtensionMap =
    {
        {".spv", AssetType::SpirVShader},
        {".mesh", AssetType::Mesh},
        {".png", AssetType::Texture},
    };

namespace Utils {

inline AssetType AssetTypeFromString(const std::string &assetType) {
  if (assetType == "None") return AssetType::None;
  if (assetType == "Mesh") return AssetType::Mesh;
  if (assetType == "Texture") return AssetType::Texture;
  if (assetType == "SpirVShader") return AssetType::SpirVShader;

  return AssetType::None;
}

inline const char *AssetTypeToString(AssetType assetType) {
  switch (assetType) {
    case AssetType::None: return "None";
    case AssetType::Mesh: return "Mesh";
    case AssetType::Texture: return "Texture";
    case AssetType::SpirVShader: return "SpirVShader";
    default:return "None";
  }
}

}
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETTYPE_H_

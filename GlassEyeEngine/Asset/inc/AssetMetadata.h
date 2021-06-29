//
// Created by user on 6/27/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_METADATA_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_METADATA_H_
#include "Asset.h"

namespace GlassEye {
class AssetMetadata {
 public:
  AssetHandle Handle;
  AssetType Type;

  std::filesystem::path FilePath;
  bool IsDataLoaded = false;
  bool IsValid() const {
    return Handle != 0;
  }
};
}
#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_METADATA_H_

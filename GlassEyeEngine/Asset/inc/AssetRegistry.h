//
// Created by user on 6/28/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETREGISTRY_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETREGISTRY_H_

// Encompasses a map of file paths to asset metadata
namespace std {

template<>
struct hash<std::filesystem::path> {
  std::size_t operator()(const std::filesystem::path &path) const {
    return hash_value(path);
  }
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_ASSET_INC_ASSETREGISTRY_H_

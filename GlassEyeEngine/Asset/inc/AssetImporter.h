//
// Created by user on 6/26/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: AssetImporter.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/26/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETIMPORTER_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETIMPORTER_H_

namespace GlassEye {
class AssetMetadata;
class Asset;
namespace AssetImporter {
void Init();
void Serialize(const Ref<class Asset> &asset);
void Serialize(const AssetMetadata &metadata, const Ref <Asset> &asset);
bool TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset);
}
};

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETIMPORTER_H_

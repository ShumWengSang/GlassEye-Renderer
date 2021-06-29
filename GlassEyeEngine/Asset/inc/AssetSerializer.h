//
// Created by user on 6/26/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETSERIALIZER_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETSERIALIZER_H_

namespace GlassEye {
class AssetSerializer {
 public:
  virtual void Serialize(const class AssetMetadata &metadata, const Ref<class Asset> &asset) const = 0;
  virtual bool TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset) const = 0;
};

class TextureSerializer : public AssetSerializer {
 public:
  virtual void Serialize(const AssetMetadata &metadata, const Ref <Asset> &asset) const override {}
  virtual bool TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset) const override;
};

class SpirVShaderSerializer : public AssetSerializer {
 public:
  virtual void Serialize(const AssetMetadata &metadata, const Ref <Asset> &asset) const override {}
  virtual bool TryLoadData(const AssetMetadata &metadata, Ref <Asset> &asset) const override;
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_CORE_INC_ASSETSERIALIZER_H_

//
// Created by user on 6/21/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: AssetManager.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/21/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "AssetManager.h"
#include "AssetManager.inl"
#include "Project.h"

namespace GlassEye {

void AssetManager::Init() {
  AssetImporter::Init();
  LoadAssetRegistry();
  FileSystem::SetChangeCallback(AssetManager::OnFileSystemChanged);
  ReloadAssets();
  WriteRegistryToFile();
}
void AssetManager::Shutdown() {
  WriteRegistryToFile();

  AssetRegistry.clear();
  LoadedAssets.clear();
}
void AssetManager::SetAssetsChangeEventFn(const AssetManager::AssetsChangeEventFn &callback) {
  this->AssetsChangeCallback = callback;
}
bool AssetManager::FileExists(AssetMetadata &metadata) {
  return FileSystem::Exists(Project::GetActive()->GetAssetDirectory() / metadata.FilePath);
}
std::string AssetManager::GetFileSystemPath(const AssetMetadata &metadata) {
  return (Project::GetAssetDirectory() / metadata.FilePath).string();
}

static AssetMetadata NullMetadata;
AssetMetadata &AssetManager::GetMetadata(AssetHandle handle) {
  for (auto&[filepath, metadata] : AssetRegistry) {
    if (metadata.Handle == handle)
      return metadata;
  }
  return NullMetadata;
}

AssetMetadata &AssetManager::GetMetadata(std::string_view filepath) {

  if (AssetRegistryContains(std::filesystem::path(filepath)))
    return AssetRegistry[filepath];

  return NullMetadata;
}
std::string AssetManager::GetRelativePath(std::string_view filepath) {
  std::string result = std::string(filepath);
  if (filepath.find(Project::GetActive()->GetAssetDirectory().string()) != std::string::npos)
    result = std::filesystem::relative(result, Project::GetActive()->GetAssetDirectory()).string();
  std::replace(result.begin(), result.end(), '\\', '/');
  return result;
}
AssetHandle AssetManager::GetAssetHandleFromFilePath(std::string_view filepath) {
  std::filesystem::path path = filepath;
  if (AssetRegistryContains(path))
    return AssetRegistry[path].Handle;

  return {};
}
AssetType AssetManager::GetAssetTypeFromExtension(std::string_view extension) {
  std::string ext = std::string(extension);
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (AssetExtensionMap.find(ext) == AssetExtensionMap.end())
    return AssetType::None;

  return AssetExtensionMap.at(ext.c_str());
}
AssetType AssetManager::GetAssetTypeFromPath(const std::filesystem::path &path) {
  return GetAssetTypeFromExtension(path.extension().string());
}
AssetHandle AssetManager::ImportAsset(std::string_view filepath) {
  return 0;
}
bool AssetManager::ReloadData(AssetHandle assetHandle) {
  auto &metadata = GetMetadata(assetHandle);
  if (!metadata.IsDataLoaded) // Data
    GLASS_CORE_WARN("Trying to reload asset that was never loaded");
  if (LoadedAssets.find(assetHandle) == LoadedAssets.end()) {
    GLASSEYE_BREAK();
  }
  Ref <Asset> &asset = LoadedAssets[assetHandle];
  metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
  return metadata.IsDataLoaded;
}
void AssetManager::LoadAssetRegistry() {
  const auto &assetRegistryPath = Project::GetAssetRegistryPath();
  if (!FileSystem::Exists(assetRegistryPath))
    return;

  std::ifstream stream(assetRegistryPath);
  std::stringstream strStream;
  strStream << stream.rdbuf();

  YAML::Node data = YAML::Load(strStream.str());
  auto handles = data["Assets"];
  if (!handles) {
    GLASS_CORE_ERROR("AssetRegistry appears to be corrupted!");
    return;
  }

  for (auto entry : handles) {
    auto filepath = entry["FilePath"].as<std::string>();

    AssetMetadata metadata;
    metadata.Handle = entry["Handle"].as<GUID_Handle>();
    metadata.FilePath = filepath;
    metadata.Type = (AssetType) Utils::AssetTypeFromString(entry["Type"].as<std::string>());

    if (metadata.Type == AssetType::None)
      continue;

    if (!FileSystem::Exists(std::string_view(AssetManager::GetFileSystemPath(metadata)))) {
      GLASS_CORE_WARN("Missing asset '{0}' detected in registry file, trying to locate...", metadata.FilePath);

      std::string mostLikelyCandiate;
      uint32_t bestScore = 0;

      for (auto &pathEntry : std::filesystem::recursive_directory_iterator(Project::GetAssetDirectory())) {
        const std::filesystem::path &path = pathEntry.path();

        if (path.filename() != metadata.FilePath.filename())
          continue;

        if (bestScore > 0)
          GLASS_CORE_WARN("Multiple candidates found...");

        std::vector<std::string> candidate_parts = Utils::SplitString(path.string(), "/\\");

        uint32_t score = 0;
        for (const auto &part : candidate_parts) {
          if (filepath.find(part) != std::string::npos)
            score++;
        }

        GLASS_CORE_WARN("'{0}' has a score of {1}, best score is {2}", path.string(), score, bestScore);

        if (bestScore > 0 && score == bestScore) {
          // TODO: How do we handle this?
        }

        if (score <= bestScore)
          continue;

        bestScore = score;
        mostLikelyCandiate = path.string();
      }

      if (mostLikelyCandiate.empty() && bestScore == 0) {
        GLASS_CORE_ERROR("Failed to locate a potential match for '{0}'", metadata.FilePath);
        continue;
      }
#ifdef GLASSEYE_WINDOWS
      std::replace(mostLikelyCandiate.begin(), mostLikelyCandiate.end(), '\\', '/');
#endif
      metadata.FilePath = std::filesystem::relative(mostLikelyCandiate, Project::GetActive()->GetAssetDirectory());
      GLASS_CORE_WARN("Found most likely match '{0}'", metadata.FilePath);
    }

    if (metadata.Handle == 0) {
      GLASS_CORE_WARN("AssetHandle for {0} is 0, this shouldn't happen.", metadata.FilePath);
      continue;
    }

    AssetRegistry[metadata.FilePath.string()] = metadata;
  }
}
void AssetManager::ProcessDirectory(const std::string &directoryPath) {
  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
    if (entry.is_directory())
      ProcessDirectory(entry.path().string());
    else
      ImportAsset(entry.path().string());
  }
}
void AssetManager::ReloadAssets() {
  ProcessDirectory(Project::GetAssetDirectory().string());
  WriteRegistryToFile();
}
void AssetManager::WriteRegistryToFile() {
  YAML::Emitter out;
  out << YAML::BeginMap;

  out << YAML::Key << "Assets" << YAML::BeginSeq;
  for (auto&[filepath, metadata] : AssetRegistry) {
    std::string pathToSerialize = metadata.FilePath.string();
#ifdef GLASSEYE_WINDOWS
    std::replace(pathToSerialize.begin(), pathToSerialize.end(), '\\', '/');
#endif
    out << YAML::BeginMap;
    out << YAML::Key << "Handle" << YAML::Value << metadata.Handle;
    out << YAML::Key << "FilePath" << YAML::Value << pathToSerialize;
    out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
    out << YAML::EndMap;
  }
  out << YAML::EndSeq;
  out << YAML::EndMap;

  const std::string &assetRegistryPath = Project::GetAssetRegistryPath().string();
  std::ofstream fout(assetRegistryPath);
  fout << out.c_str();
}
void AssetManager::OnFileSystemChanged(FileSystemChangedEvent e) {
  e.FilePath = (Project::GetAssetDirectory() / e.FilePath).string();
#ifdef GLASSEYE_WINDOWS
  std::string temp = e.FilePath.string();
  std::replace(temp.begin(), temp.end(), '\\', '/');
  e.FilePath = temp;
#endif
  e.NewName = Utils::RemoveExtension(e.NewName);
  AssetManager &assetManager = AssetManager::Get();
  if (!e.IsDirectory) {
    switch (e.Action) {
      case FileSystemAction::Added:assetManager.ImportAsset(e.FilePath.string());
        break;
      case FileSystemAction::Delete:assetManager.OnAssetDeleted(assetManager.GetAssetHandleFromFilePath(e.FilePath.string()));
        break;
      case FileSystemAction::Modified:
        // TODO: Reload data if loaded
        break;
      case FileSystemAction::Rename: {
        AssetType previousType = assetManager.GetAssetTypeFromPath(e.OldName);
        AssetType newType = assetManager.GetAssetTypeFromPath(e.FilePath);

        if (previousType == AssetType::None && newType != AssetType::None) {
          assetManager.ImportAsset(e.FilePath.string());
        } else {
          assetManager.OnAssetRenamed(assetManager.GetAssetHandleFromFilePath((e.FilePath.parent_path()
              / e.OldName).string()), e.FilePath.string());
          e.WasTracking = true;
        }
        break;
      }
    }
  }
  assetManager.AssetsChangeCallback(e);
}

void AssetManager::OnAssetRenamed(AssetHandle assetHandle, const std::string &newFilePath) {
  AssetMetadata metadata = GetMetadata(assetHandle);
  AssetRegistry.erase(metadata.FilePath);
  metadata.FilePath = newFilePath;
  AssetRegistry[metadata.FilePath] = metadata;
  WriteRegistryToFile();
}
void AssetManager::OnAssetMoved(AssetHandle assetHandle, const std::string &destinationPath) {
  AssetMetadata assetInfo = GetMetadata(assetHandle);

  AssetRegistry.erase(assetInfo.FilePath);
  assetInfo.FilePath = destinationPath / assetInfo.FilePath.filename();
  AssetRegistry[assetInfo.FilePath] = assetInfo;
  WriteRegistryToFile();
}
void AssetManager::OnAssetDeleted(AssetHandle assetHandle) {
  AssetMetadata metadata = GetMetadata(assetHandle);
  AssetRegistry.erase(metadata.FilePath);
  LoadedAssets.erase(assetHandle);

  WriteRegistryToFile();
}
bool AssetManager::AssetRegistryContains(const std::filesystem::path &filepath) const {
  return AssetRegistry.find(filepath) != AssetRegistry.end();
}

}
//
// Created by user on 6/27/2021.
//
#include "EnginePCH.h"
#include "Project.h"
#include "AssetManager.h"

const GlassEye::ProjectConfig &GlassEye::Project::GetConfig() const { return m_Config; }
GlassEye::Ref<GlassEye::Project> GlassEye::Project::GetActive() { return ActiveProject; }
const std::string &GlassEye::Project::GetProjectName() {
  return ActiveProject->GetConfig().Name;
}
std::filesystem::path GlassEye::Project::GetProjectDirectory() {
  return ActiveProject->GetConfig().ProjectDirectory;
}
std::filesystem::path GlassEye::Project::GetAssetDirectory() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory) / ActiveProject->GetConfig().AssetDirectory;
}
std::filesystem::path GlassEye::Project::GetAssetRegistryPath() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory)
      / ActiveProject->GetConfig().AssetRegistryPath;
}
std::filesystem::path GlassEye::Project::GetMeshPath() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory) / ActiveProject->GetConfig().MeshPath;
}
std::filesystem::path GlassEye::Project::GetScriptModulePath() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory)
      / ActiveProject->GetConfig().ScriptModulePath;
}
std::filesystem::path GlassEye::Project::GetScriptModuleFilePath() {
  return GetScriptModulePath() / fmt::format("{0}.dll", GetProjectName());
}
std::filesystem::path GlassEye::Project::GetCacheDirectory() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory) / "Cache";
}

void GlassEye::Project::SetActive(GlassEye::Ref<GlassEye::Project> project) {
  ActiveProject = project;
  AssetManager::Get().Init();
}
void GlassEye::Project::OnDeserialized() {

}
std::filesystem::path GlassEye::Project::GetShaderPath() {
  return std::filesystem::path(ActiveProject->GetConfig().ProjectDirectory)
      / ActiveProject->GetConfig().ShaderDirectory;
}

//
// Created by user on 6/27/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_PROJECT_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_PROJECT_H_

namespace GlassEye {
struct ProjectConfig {
  std::string Name;

  std::string AssetDirectory;
  std::string AssetRegistryPath;

  std::string MeshPath;
  std::string MeshSourcePath;

  std::string ScriptModulePath;
  std::string ShaderDirectory;
  std::string StartScene;

  // Not serialized
  std::string ProjectDirectory;
};

class Project {
 public:
  Project() = default;
  ~Project() = default;

  const ProjectConfig &GetConfig() const;

  static Ref <Project> GetActive();
  static void SetActive(Ref <Project> project);

  static const std::string &GetProjectName();

  static std::filesystem::path GetProjectDirectory();

  static std::filesystem::path GetAssetDirectory();

  static std::filesystem::path GetAssetRegistryPath();

  static std::filesystem::path GetMeshPath();

  static std::filesystem::path GetScriptModulePath();

  static std::filesystem::path GetScriptModuleFilePath();

  static std::filesystem::path GetCacheDirectory();

  static std::filesystem::path GetShaderPath();
 private:
  void OnDeserialized();
 private:
  ProjectConfig m_Config;

  friend class ProjectSerializer;

  inline static Ref <Project> ActiveProject;
};
}
#endif //GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_PROJECT_H_

//
// Created by user on 6/27/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: FileSystem.cpp
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/27/2021
 * End Header --------------------------------------------------------*/
#include "EnginePCH.h"
#include "FileSystem.h"
#include <Utils.h>
#include <shellapi.h>

bool GlassEye::FileSystem::CreateDirectory(const std::filesystem::path &directory) {
  return std::filesystem::create_directories(directory);
}
bool GlassEye::FileSystem::CreateDirectory(std::string_view directory) {
  return CreateDirectory(std::filesystem::path(directory));
}
bool GlassEye::FileSystem::Exists(const std::filesystem::path &filepath) {
  return std::filesystem::exists(filepath);
}
bool GlassEye::FileSystem::Exists(std::string_view filepath) {
  return Exists(std::filesystem::path(filepath));
}

bool GlassEye::FileSystem::IsDirectory(std::string_view filepath) {
  bool result = std::filesystem::is_directory(filepath);

  if (!result)
    result = Utils::GetExtension(filepath).empty();

  return result;
}
bool GlassEye::FileSystem::ShowFileInExplorer(const std::filesystem::path &path) {
  auto absolutePath = std::filesystem::canonical(path);
  if (!Exists(absolutePath))
    return false;

  std::string cmd = fmt::format("explorer.exe /select,\"{0}\"", absolutePath.string());
  system(cmd.c_str());
  return true;
}

void GlassEye::FileSystem::SetChangeCallback(const GlassEye::FileSystem::FileSystemChangedCallbackFn &callback) {
  Callback = callback;
}


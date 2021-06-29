//
// Created by user on 6/27/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: FileSystem.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/27/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_FILESYSTEM_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_FILESYSTEM_H_
namespace GlassEye {
enum class FileSystemAction {
  Added, Rename, Modified, Delete
};

struct FileSystemChangedEvent {
  FileSystemAction Action;
  std::filesystem::path FilePath;
  std::string OldName;
  std::string NewName;
  bool IsDirectory;
  bool WasTracking = false;
};

class FileSystem {
 public:
  static bool CreateDirectory(const std::filesystem::path &directory);
  static bool CreateDirectory(std::string_view directory);
  static bool Exists(const std::filesystem::path &filepath);
  static bool Exists(std::string_view filepath);
  static std::string Rename(std::string const &filepath, std::string_view newName);
  static bool DeleteFile(std::string const &filepath);
  static bool MoveFile(std::string const &filepath, std::string_view dest);
  static bool IsDirectory(std::string_view filepath);

  static bool ShowFileInExplorer(const std::filesystem::path &path);
  static bool OpenDirectoryInExplorer(const std::filesystem::path &path);
  static bool OpenExternally(const std::filesystem::path &path);
 public:
  using FileSystemChangedCallbackFn = std::function<void(FileSystemChangedEvent)>;

  static void SetChangeCallback(const FileSystemChangedCallbackFn &callback);
  static void StartWatching();
  static void StopWatching();

  static void SkipNextFileSystemChange();
 private:
  static unsigned long Watch();
 private:
  static inline FileSystemChangedCallbackFn Callback;
  static inline bool IgnoreNextChange = false;
  static inline bool Watching = false;
};
}

#endif //GLASSEYEENGINE_GLASSEYEENGINE_UTILS_INC_FILESYSTEM_H_

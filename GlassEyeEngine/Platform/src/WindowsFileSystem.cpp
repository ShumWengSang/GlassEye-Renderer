//
// Created by user on 6/27/2021.
//

#include "FileSystem.h"
#include <ShellAPI.h>
#include "Project.h"

/////////////////////// VARIABLES ////////////////
static std::thread localThread;

////////////////////////////////////////////////



bool GlassEye::FileSystem::OpenDirectoryInExplorer(const std::filesystem::path &path) {
  auto absolutePath = std::filesystem::canonical(path);
  if (!Exists(absolutePath))
    return false;

  ShellExecute(NULL, "explore", absolutePath.string().c_str(), NULL, NULL, SW_SHOWNORMAL);
  return true;
}
bool GlassEye::FileSystem::OpenExternally(const std::filesystem::path &path) {
  auto absolutePath = std::filesystem::canonical(path);
  if (!Exists(absolutePath))
    return false;

  ShellExecute(NULL, "open", absolutePath.string().c_str(), NULL, NULL, SW_SHOWNORMAL);
  return true;
}

std::string GlassEye::FileSystem::Rename(std::string const &filepath, std::string_view newName) {
  IgnoreNextChange = true;
  std::filesystem::path p = filepath;
  std::string newFilePath = p.parent_path().string() + "/" + std::string(newName) + p.extension().string();
  MoveFileA(filepath.c_str(), newFilePath.c_str());
  IgnoreNextChange = false;
  return newFilePath;
}
bool GlassEye::FileSystem::DeleteFile(std::string const &filepath) {
  IgnoreNextChange = true;
  std::string fp = filepath;
  fp.append(1, '\0');
  SHFILEOPSTRUCTA file_op;
  file_op.hwnd = NULL;
  file_op.wFunc = FO_DELETE;
  file_op.pFrom = fp.c_str();
  file_op.pTo = "";
  file_op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
  file_op.fAnyOperationsAborted = false;
  file_op.hNameMappings = 0;
  file_op.lpszProgressTitle = "";
  int result = SHFileOperationA(&file_op);
  IgnoreNextChange = false;
  return result == 0;
}
bool GlassEye::FileSystem::MoveFile(std::string const &filepath, std::string_view dest) {
  IgnoreNextChange = true;
  std::filesystem::path p = filepath;
  std::string destFilePath = std::string(dest) + "/" + p.filename().string();
  BOOL result = MoveFileA(filepath.c_str(), destFilePath.c_str()) != 0;
  IgnoreNextChange = false;
  return result;
}
void GlassEye::FileSystem::StartWatching() {
  if (!Watching) {
    Watching = true;
    localThread = std::thread(Watch);
  }
}
void GlassEye::FileSystem::StopWatching() {
  if (Watching) {
    Watching = false;
    localThread.join();
  }
}
void GlassEye::FileSystem::SkipNextFileSystemChange() {
  IgnoreNextChange = true;
}

unsigned long GlassEye::FileSystem::Watch() {
  std::string assetDirectory = Project::GetActive()->GetAssetDirectory().string();
  std::vector<BYTE> buffer;
  buffer.resize(10 * 1024);
  OVERLAPPED overlapped = {0};
  HANDLE handle = NULL;
  DWORD bytesReturned = 0;

  handle = CreateFileA(
      assetDirectory.c_str(),
      FILE_LIST_DIRECTORY,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
      NULL
  );

  ZeroMemory(&overlapped, sizeof(overlapped));

  if (handle == INVALID_HANDLE_VALUE)
    GLASS_CORE_ERROR("Unable to accquire directory handle: {0}", GetLastError());

  overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  if (overlapped.hEvent == NULL) {
    GLASS_CORE_ERROR("CreateEvent failed!");
    return 0;
  }

  while (Watching) {
    DWORD status = ReadDirectoryChangesW(
        handle,
        &buffer[0],
        (DWORD) buffer.size(),
        TRUE,
        FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
        &bytesReturned,
        &overlapped,
        NULL
    );

    if (!status)
      GLASS_CORE_ERROR(GetLastError());

    DWORD waitOperation = WaitForSingleObject(overlapped.hEvent, 5000);
    if (waitOperation != WAIT_OBJECT_0)
      continue;

    if (IgnoreNextChange) {
      IgnoreNextChange = false;
      continue;
    }

    std::string oldName;
    char fileName[MAX_PATH * 10] = "";

    BYTE *buf = buffer.data();
    for (;;) {
      FILE_NOTIFY_INFORMATION &fni = *(FILE_NOTIFY_INFORMATION *) buf;
      ZeroMemory(fileName, sizeof(fileName));
      WideCharToMultiByte(CP_ACP,
                          0,
                          fni.FileName,
                          fni.FileNameLength / sizeof(WCHAR),
                          fileName,
                          sizeof(fileName),
                          NULL,
                          NULL);
      std::filesystem::path filepath = std::string(fileName);

      FileSystemChangedEvent e;
      e.FilePath = filepath;
      e.NewName = filepath.filename().string();
      e.OldName = filepath.filename().string();
      e.IsDirectory = IsDirectory(e.FilePath.string());

      switch (fni.Action) {
        case FILE_ACTION_ADDED: {
          e.Action = FileSystemAction::Added;
          Callback(e);
          break;
        }
        case FILE_ACTION_REMOVED: {
          e.Action = FileSystemAction::Delete;
          Callback(e);
          break;
        }
        case FILE_ACTION_MODIFIED: {
          e.Action = FileSystemAction::Modified;
          Callback(e);
          break;
        }
        case FILE_ACTION_RENAMED_OLD_NAME: {
          oldName = filepath.filename().string();
          break;
        }
        case FILE_ACTION_RENAMED_NEW_NAME: {
          e.OldName = oldName;
          e.Action = FileSystemAction::Rename;
          Callback(e);
          break;
        }
      }

      if (!fni.NextEntryOffset)
        break;

      buf += fni.NextEntryOffset;
    }
  }

  return 0;
}
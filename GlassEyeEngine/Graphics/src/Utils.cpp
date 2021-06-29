//
// Created by user on 6/26/2021.
//

#include "EnginePCH.h"
#include "Utils.h"

std::string GlassEye::Utils::GetFilename(std::string_view filepath) {
  std::vector<std::string> parts = SplitString(filepath, "/\\");

  if (parts.size() > 0)
    return parts[parts.size() - 1];

  return "";
}
std::string GlassEye::Utils::GetExtension(std::string_view filename) {
  std::vector<std::string> parts = SplitString(filename, '.');

  if (parts.size() > 1)
    return parts[parts.size() - 1];

  return "";
}
std::string GlassEye::Utils::RemoveExtension(std::string_view filename) {
  return std::string(filename.substr(0, filename.find_last_of('.')));
}
bool GlassEye::Utils::StartsWith(std::string_view string, std::string_view start) {
  return string.find(start) == 0;
}
std::vector<std::string> GlassEye::Utils::SplitString(std::string_view string, std::string_view delimiters) {
  size_t start = 0;
  size_t end = string.find_first_of(delimiters);

  std::vector<std::string> result;

  while (end <= std::string_view::npos) {
    std::string_view token = string.substr(start, end - start);
    if (!token.empty())
      result.emplace_back(std::string(token));

    if (end == std::string_view::npos)
      break;

    start = end + 1;
    end = string.find_first_of(delimiters, start);
  }

  return result;
}
std::vector<std::string> GlassEye::Utils::SplitString(std::string_view string, const char delimiter) {
  return SplitString(string, std::string(1, delimiter));
}

//
// Created by user on 6/26/2021.
//

/* Start Header -------------------------------------------------------
 * File Name: Utils.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Project: GlassEyeEngine
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 6/26/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_UTILS_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_UTILS_H_

// Uitlity stuff grabbed from Cherno, https://www.youtube.com/user/TheChernoProject
namespace GlassEye::Utils {
std::string GetFilename(std::string_view filepath);
std::string GetExtension(std::string_view filename);
std::string RemoveExtension(std::string_view filename);
bool StartsWith(std::string_view string, std::string_view start);
std::vector<std::string> SplitString(std::string_view string, std::string_view delimiters);
std::vector<std::string> SplitString(std::string_view string, const char delimiter);
};

#endif //GLASSEYEENGINE_GLASSEYEENGINE_GRAPHICS_INC_UTILS_H_

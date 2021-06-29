//
// Created by user on 5/26/2021.
//

/* Start Header -------------------------------------------------------
 * Copyright (C) 2020 DigiPen Institute of Technology.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 * File Name: Logger.cpp.cc
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEye
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 5/26/2021
 * End Header --------------------------------------------------------*/
#include "Logger.h"

void GlassEye::Logger::Init() {
  spdlog::set_pattern("$^[%T] %n: %v%$");
  coreLogger = spdlog::stdout_color_mt("GLASS EYE CORE");
  coreLogger->set_level(spdlog::level::trace);
  clientLogger = spdlog::stdout_color_mt("APP");
  clientLogger->set_level(spdlog::level::trace);
}
std::shared_ptr<spdlog::logger> &GlassEye::Logger::GetCoreLogger() {
  return coreLogger;
}
std::shared_ptr<spdlog::logger> &GlassEye::Logger::GetClientLogger() {
  return clientLogger;
}

//
// Created by user on 5/26/2021.
//

/* Start Header -------------------------------------------------------
 * Copyright (C) 2020 DigiPen Institute of Technology.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 * File Name: Logger.h
 * Purpose: Lorem ipsum dolor sit amet, consectetur adipiscing elit.
 * Language: C++, G++
 * Platform: g++ (Ubuntu 9.3.0-10ubuntu2) 9.3, ThinkPad T430u, Nvidia GT 620M,
 *           OpenGL version string: 4.6.0 NVIDIA 390.138
 * Project: GlassEye
 * Author: Roland Shum, roland.shum@digipen.edu
 * Creation date: 5/26/2021
 * End Header --------------------------------------------------------*/


#ifndef GLASSEYE_SOURCE_SRC_LOGGER_H_
#define GLASSEYE_SOURCE_SRC_LOGGER_H_

namespace GlassEye {
class Logger {
 public:
  static void Init();
  inline static std::shared_ptr<spdlog::logger>& GetCoreLogger();
  inline static std::shared_ptr<spdlog::logger>& GetClientLogger();

 private:
  static inline std::shared_ptr<spdlog::logger> coreLogger;
  static inline std::shared_ptr<spdlog::logger> clientLogger;
};
}

#define GLASS_CORE_TRACE(...)     ::GlassEye::GetCoreLogger()->trace(__VA_ARGS__)
#define GLASS_CORE_INFO(...)      ::GlassEye::GetCoreLogger()->info(__VA_ARGS__)
#define GLASS_CORE_WARN(...)      ::GlassEye::GetCoreLogger()->warn(__VA_ARGS__)
#define GLASS_CORE_ERROR(...)     ::GlassEye::GetCoreLogger()->error(__VA_ARGS__)
#define GLASS_CORE_CRITICAL(...)  ::GlassEye::GetCoreLogger()->critical(__VA_ARGS__)

#define GLASS_TRACE(...)         ::GlassEye::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GLASS_INFO(...)          ::GlassEye::Log::GetClientLogger()->info(__VA_ARGS__)
#define GLASS_WARN(...)          ::GlassEye::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GLASS_ERROR(...)         ::GlassEye::Log::GetClientLogger()->error(__VA_ARGS__)
#define GLASS_CRITICAL(...)      ::GlassEye::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif //GLASSEYE_SOURCE_SRC_LOGGER_H_

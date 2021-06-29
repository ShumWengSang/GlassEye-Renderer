//
// Created by user on 5/26/2021.
//

/* Start Header -------------------------------------------------------
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
  static std::shared_ptr<spdlog::logger> &GetCoreLogger();
  static std::shared_ptr<spdlog::logger> &GetClientLogger();

 private:
  static inline std::shared_ptr<spdlog::logger> coreLogger;
  static inline std::shared_ptr<spdlog::logger> clientLogger;
};
}

#define GLASS_CORE_TRACE(...)     ::GlassEye::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define GLASS_CORE_INFO(...)      ::GlassEye::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define GLASS_CORE_WARN(...)      ::GlassEye::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define GLASS_CORE_ERROR(...)     ::GlassEye::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define GLASS_CORE_CRITICAL(...)  ::GlassEye::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define GLASS_TRACE(...)         ::GlassEye::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define GLASS_INFO(...)          ::GlassEye::Logger::GetClientLogger()->info(__VA_ARGS__)
#define GLASS_WARN(...)          ::GlassEye::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define GLASS_ERROR(...)         ::GlassEye::Logger::GetClientLogger()->error(__VA_ARGS__)
#define GLASS_CRITICAL(...)      ::GlassEye::Logger::GetClientLogger()->critical(__VA_ARGS__)

#endif //GLASSEYE_SOURCE_SRC_LOGGER_H_

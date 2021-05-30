//
// Created by user on 5/28/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORM_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORM_H_

#ifdef _DEBUG
 #define GLASSEYE_DEBUG
#endif
#ifdef _WIN32

#define GLASSEYE_APPLE			0
#define GLASSEYE_WINDOWS		1
#define GLASSEYE_LINUX			0

#include "PlatformWindows.h"

#elif __APPLE__

#define GLASSEYE_APPLE			1
#define GLASSEYE_WINDOWS		0
#define GLASSEYE_LINUX			0

#error Fling does not support Apple

#elif __linux__

#define GLASSEYE_APPLE			0
#define GLASSEYE_WINDOWS		0
#define GLASSEYE_LINUX			1

// #include "PlatformLinux.h"
# error "Linux is on the todo list. Haha.."
#else

#   error "Unknown compiler"

#endif
#endif //GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORM_H_

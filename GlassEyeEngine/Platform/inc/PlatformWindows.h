//
// Created by user on 5/28/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_

// We assume Windows means MSVC. Bad things might happen if it is not.
#ifdef GLASSEYE_WINDOWS
#define NOMINMAX
#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#undef CreateDirectory
#undef DeleteFile
#undef MoveFile
#define GLASSEYE_BREAK() __debugbreak();
#endif

#endif //GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_

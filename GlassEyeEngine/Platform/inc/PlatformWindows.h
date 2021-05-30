//
// Created by user on 5/28/2021.
//

#ifndef GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_
#define GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_

// We assume Windows means MSVC. Bad things might happen if it is not.
#ifdef GLASSEYE_WINDOWS
  #define NOMINMAX
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <stdlib.h>
  #include <crtdbg.h>
  #define GLASSEYE_BREAK() __debugbreak();
#endif

#endif //GLASSEYEENGINE_GLASSEYEENGINE_PLATFORM_PLATFORMWINDOWS_H_

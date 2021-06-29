#pragma once

#include "SystemIncludes.h"
#include "ThirdPartyIncludes.h"
#include "Types.h"
#include "Platform.h"
#include "Logger.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(x) (void)x
#endif

#define    EPSILON     0.00001f
#define    PI          3.1415926f
#define    HALF_PI     (PI * 0.5f)
#define    TWO_PI      (PI * 2.0f)

#define BIT(x) (1 << x)
#define GLASSEYE_BIND_EVENT_FN(x) std::bind(&##fn, std::placeholders::_1)
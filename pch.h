// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#define DLL_NAME "gameShieldDll.dll"

#define DLL_EXPORT extern "C" __declspec(dllexport)
//#define BASE_ADDRESS_32BIT 0x400000
#define BASE_ADDRESS_32BIT (1 << 22)

#include "enums.h"
#include "types.h"
#include "config.h"

#endif //PCH_H

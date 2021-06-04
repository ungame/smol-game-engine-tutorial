#ifndef SMOL_ENGINE
#define SMOL_ENGINE

#include <smol/smol.h>

#ifdef SMOL_PLATFORM_WINDOWS
    #ifdef SMOL_ENGINE_IMPLEMENTATION
        #define SMOL_ENGINE_API __declspec(dllexport)
    #else
        #define SMOL_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define SMOL_ENGINE_API
#endif

#endif
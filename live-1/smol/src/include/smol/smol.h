#ifndef SMOL_H
#define SMOL_H

#include "smol_version.h"

// easier to check platform independent debug flag
#if defined(DEBUG) || defined(_DEBUG)
    #define SMOL_DEBUG
#endif

#ifdef _WIN32
    #define SMOL_PLATFORM_WINDOWS
    #define _CRT_SECURE_NO_WARNINGS
 #else
    #define SMOL_PLATFORM_UNKNOWN
    #error "Plataforma não suportada"
#endif

#endif //SMOL_H

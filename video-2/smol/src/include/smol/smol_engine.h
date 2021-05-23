#ifndef SMOL_ENGINE
#define SMOL_ENGINE

#include <smol/smol_platform.h>

#ifdef SMOL_PLATFORM_WINDOWS
    #ifdef SMOL_ENGINE_IMPLEMENTATION
        #define SMOL_ENGINE_API __declspec(dllexport)
    #else
        #define SMOL_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define SMOL_ENGINE_API
#endif

namespace smol 
{
    class SMOL_ENGINE_API Engine final 
    {
        public:
            Platform platorm;

            Engine();
            ~Engine();
    };
}

#endif
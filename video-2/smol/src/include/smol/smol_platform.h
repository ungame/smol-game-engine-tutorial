#ifndef SMOL_PLATFORM_H
#define SMOL_PLATFORM_H

#ifdef _WIN32
    #include <windows.h>
    #define SMOL_PLATFORM_WINDOWS
#else
    #define SMOL_PLATFORM_UNKNOWN
    #error "Plataforma nao suportada"
#endif

#ifdef SMOL_PLATFORM_WINDOWS
    #ifdef SMOL_PLATFORM_IMPLEMENTATION
        #define SMOL_PLATFORM_API __declspec(dllexport)
    #else
        #define SMOL_PLATFORM_API __declspec(dllimport)
    #endif
#else
    #define SMOL_PLATFORM_API
#endif


namespace smol 
{
    class SMOL_PLATFORM_API Platform final 
    {
        public:
            void showMessage(char *message);
    };
}

#endif
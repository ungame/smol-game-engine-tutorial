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
    struct Window;

    struct SMOL_PLATFORM_API Platform final 
    {
        void showMessage(char *message);

        Window* createWindow(int width, int height, const char* title);

        void updateWindowEvents(Window* window);

        bool getWindowCloseFlag(Window* window);

        void clearWindowCloseFlag(Window* window);

        void destroyWindow(Window* window);

        bool initOpenGL(Window *window, int glVersionMajor, int glVersionMinor, int colorBits = 32, int depthBits = 24);
    };
}

#endif
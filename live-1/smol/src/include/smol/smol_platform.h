#ifndef SMOL_PLATFORM_H
#define SMOL_PLATFORM_H

#ifdef SMOL_PLATFORM_WINDOWS
#include <windows.h>
#endif

#ifdef SMOL_PLATFORM_WINDOWS
    #ifdef SMOL_ENGINE_IMPLEMENTATION
        #define SMOL_PLATFORM_API __declspec(dllexport)
    #else
        #define SMOL_PLATFORM_API __declspec(dllimport)
    #endif //SMOL_ENGINE_IMPLEMENTATION
#else
    #define SMOL_PLATFORM_API
#endif // SMOL_PLATFORM_WINDOWS


namespace smol 
{
    struct Window;
    struct Module;

    struct KeyboardState
    {
        enum
        {
            PRESSED_BIT = 1,
            CHANGED_THIS_FRAME_BIT = 1 << 1,
            MAX_KEYS = 256
        };

        unsigned char key[MAX_KEYS];
    };

    struct SMOL_PLATFORM_API Platform final 
    {
        static void showMessage(char *message);

        static Window* createWindow(int width, int height, const char* title);

        static void updateWindowEvents(Window* window);

        static bool getWindowCloseFlag(Window* window);

        static void clearWindowCloseFlag(Window* window);

        static void destroyWindow(Window* window);

        static bool initOpenGL(int glVersionMajor, int glVersionMinor, int colorBits = 32, int depthBits = 24);

        static Module* loadModule(const char* path);

        static bool unloadModule(Module* module);

        static void* getFunctionFromModule(Module* module, const char* function);

        static const unsigned char* getKeyboardState();

        static char* loadFileToBuffer(const char* fileName, size_t* loadedFileSize = nullptr, size_t extraBytes = 0, size_t offset = 0);

        static char* loadFileToBufferNullTerminated(const char* fileName, size_t* fileSize = nullptr);

        static void unloadFileBuffer(const char* fileBuffer);

        static const char* getBinaryPath();
    };
}

#endif
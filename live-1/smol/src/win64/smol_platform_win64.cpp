#include <smol/smol.h>
#define SMOL_GL_DEFINE_EXTERN
#include <smol/smol_gl.h>
#include <smol/smol_version.h>
#include <smol/smol_platform.h>
#include <smol/smol_log.h>
#include <cstdio>

namespace smol 
{

    constexpr UINT SMOL_CLOSE_WINDOW = WM_USER + 1;
    constexpr INT SMOL_DEFAULT_ICON_ID = 101;

    struct Window 
    {
        HWND handle;
        HDC dc;
        HGLRC rc;
        bool shouldClose = false;
        static KeyboardState keyboardState;
    };

    KeyboardState Window::keyboardState = {};

    struct Module
    {
        HMODULE handle;
    };

    static struct RenderAPIInfo
    {
        enum APIName
        {
            NONE,
            OPENGL 
        };

        struct OpenGL
        {
            HGLRC sharedContext;
            unsigned int versionMajor;
            unsigned int versionMinor;

            PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

            int pixelFormatAttribs[16];
            int contextAttribs[16];
        };
       
       APIName name;
       OpenGL gl;
    } globalRenderApiInfo;

    LRESULT smolWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
    {
        switch(uMsg) 
        {
            case WM_CLOSE:

                PostMessage(hwnd, SMOL_CLOSE_WINDOW, 0, 0);

                break;

            // Ref: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
            case WM_KEYDOWN:

            // Ref: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
            case WM_KEYUP:
                {
                    int isDown = !(lParam & (1 << 31)); // 0 = pressed, 1 = released
                    int wasDown = (lParam & (1 << 30)) != 0;
                    int state = (((isDown ^ wasDown) << 1) | isDown);
                    int vkCode = (short) wParam;
                    
                    Window::keyboardState.key[vkCode] = (unsigned char) state;

                    break;
                }

            default:
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        return 0;
    };

    void Platform::showMessage(char* message) 
    {
        char title[64];
        snprintf(title, 64, "SMOL Engine v%s", smol::SMOL_VERSION);
        MessageBox(0, message, title, 0);
    }


    bool Platform::initOpenGL(int glVersionMajor, int glVersionMinor, int colorBits, int depthBits)
    {
        Window* dummyWindow = createWindow(0, 0, "Dummy Window");

        PIXELFORMATDESCRIPTOR pfd = { 
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW |
            PFD_SUPPORT_OPENGL |
            PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            (BYTE) depthBits,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            (BYTE) colorBits,
            0,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        int pixelFormat = ChoosePixelFormat(dummyWindow->dc, &pfd);

        if(! pixelFormat )
        {
            smol::Log::error("Nao foi possivel alocar um pixel format");
            destroyWindow(dummyWindow);
            return false;
        }        

        if(! SetPixelFormat(dummyWindow->dc, pixelFormat, &pfd) ) 
        {
            smol::Log::error("Nao foi possivel setar o pixel format");
            destroyWindow(dummyWindow);
            return false;
        }

        HGLRC rc = wglCreateContext(dummyWindow->dc);
        if (! rc )
        {
            smol::Log::error("Nao foi possivel criar um contexto OpenGL valido");
            destroyWindow(dummyWindow);
            return false;
        }

        if (! wglMakeCurrent(dummyWindow->dc, rc) ) 
        {
            smol::Log::error("Nao foi possivel setar o context OpenGL atual");
            destroyWindow(dummyWindow);
            return false;  
        }
        

        const int pixelFormatAttribList[] = { 
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_COLOR_BITS_ARB, colorBits,
            WGL_DEPTH_BITS_ARB, depthBits,
            0
        };

        const int contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, glVersionMajor,
            WGL_CONTEXT_MINOR_VERSION_ARB, glVersionMinor,
            WGL_CONTEXT_FLAGS_ARB,

            #ifdef SMOL_DEBUG
            WGL_CONTEXT_DEBUG_BIT_ARB |
            #endif

            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        globalRenderApiInfo.name = RenderAPIInfo::APIName::OPENGL;
        globalRenderApiInfo.gl.versionMajor = glVersionMajor;
        globalRenderApiInfo.gl.versionMinor = glVersionMinor;
        globalRenderApiInfo.gl.wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
        globalRenderApiInfo.gl.wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
        
        memcpy(globalRenderApiInfo.gl.pixelFormatAttribs, pixelFormatAttribList, sizeof(pixelFormatAttribList));
        memcpy(globalRenderApiInfo.gl.contextAttribs, contextAttribs, sizeof(contextAttribs));

        wglMakeCurrent(0, 0);
        wglDeleteContext(rc);
        destroyWindow(dummyWindow);

        return true;
    }


    Window* Platform::createWindow(int width, int height, const char* title)
    {

        const char* smolWindowClass = "SMOL_WINDOW_CLASS";

        WNDCLASSEXA wc = {};

        HINSTANCE hInstance = GetModuleHandleA(NULL);

        if(! GetClassInfoExA(hInstance, smolWindowClass, &wc) )
        {
            wc.cbSize = sizeof(WNDCLASSEXA);
            wc.style = CS_OWNDC;
            wc.lpfnWndProc = smolWindowProc;
            wc.hInstance = hInstance;
            wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCE(SMOL_DEFAULT_ICON_ID));
            wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
            wc.lpszClassName = smolWindowClass;

            if (! RegisterClassExA(&wc) )
            {
                smol::Log::error("Nao foi possivel registrar a classe da janela");
                return nullptr;
            }
        }

        HWND windowHandle = CreateWindowExA(
            0,
            smolWindowClass,
            title,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            NULL,
            NULL,
            hInstance,
            NULL
        );

        if (! windowHandle )
        {
            smol::Log::error("Nao foi possivel criar uma janela");
            return nullptr;
        }

        Window* window = new Window;
        window->handle = windowHandle;
        window->dc = GetDC(windowHandle);
 
        if(globalRenderApiInfo.name == RenderAPIInfo::APIName::OPENGL) {

            int pixelFormat; 
            int numPixelFormats = 0;
            PIXELFORMATDESCRIPTOR pfd;

            const int* pixelFormatAttribList = (const int*) globalRenderApiInfo.gl.pixelFormatAttribs;
            const int* contextAttribList = (const int*) globalRenderApiInfo.gl.contextAttribs;

            globalRenderApiInfo.gl.wglChoosePixelFormatARB(window->dc, pixelFormatAttribList, nullptr, 1, &pixelFormat, (UINT*) &numPixelFormats);

            if( numPixelFormats <= 0 )
            {
                smol::Log::error("Nao foi possivel pegar um pixel format valido");
                return nullptr;          
            }


            if(! SetPixelFormat(window->dc, pixelFormat, &pfd) )
            {
                smol::Log::error("Nao foi possivel setar o pixel format");
                return nullptr;
            }

            HGLRC sharedContext = globalRenderApiInfo.gl.sharedContext;
            HGLRC rc = globalRenderApiInfo.gl.wglCreateContextAttribsARB(window->dc, 0, contextAttribList);

            bool mustGetGLFunctions = false;
            if (! sharedContext )
            {
                globalRenderApiInfo.gl.sharedContext = rc;
                mustGetGLFunctions = true;
            }   

            if(! rc )
            {
                smol::Log::error("Nao foi possivel criar um contexto OpenGL valido");
                return nullptr;
            }

            window->rc = rc;
            if(! wglMakeCurrent(window->dc, window->rc) )
            {
                smol::Log::error("Não foi possivel setar o contexto OpenGL atual");
                return nullptr;
            }

            if(mustGetGLFunctions) {
                getOpenGLFunctionPointers();
            }
        }

        return window;
    }

    void Platform::updateWindowEvents(Window* window)
    {

        MSG msg;
        HWND hwnd = window->handle;

        if(globalRenderApiInfo.name == RenderAPIInfo::APIName::OPENGL) {
            wglMakeCurrent(window->dc, window->rc);
        }

        // clean up change bit for keyboard state
        for (int keyCode = 0; keyCode < smol::KeyboardState::MAX_KEYS; keyCode++)
        {
            Window::keyboardState.key[keyCode] &= ~smol::KeyboardState::CHANGED_THIS_FRAME_BIT;
        }

        while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) 
        {
            if(msg.message == SMOL_CLOSE_WINDOW) 
            {

                window->shouldClose = true;

                return;

            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
            
        SwapBuffers(window->dc);

    }

    bool Platform::getWindowCloseFlag(Window* window)
    {
        return window->shouldClose;
    }

    void Platform::clearWindowCloseFlag(Window* window)
    {
        window->shouldClose = false;
    }

    void Platform::destroyWindow(Window* window)
    {
        wglDeleteContext(window->rc);
        DeleteDC(window->dc);
        DestroyWindow(window->handle);
        delete window;
    }

    Module* Platform::loadModule(const char* path) {
        HMODULE hmodule = LoadLibrary(path);
        if(hmodule)
        {
            Module* module = new Module;
            module->handle = hmodule;
            return module;
        }

        smol::Log::error("Falhou ao carregar modulo: %s", path);
        return nullptr;
    }

    bool Platform::unloadModule(Module* module) {
        if(! FreeLibrary(module->handle) )
        {
            smol::Log::error("Erro ao descarregar modulo");
            return false;
        }
        delete module;
        return true;
    }

    void* Platform::getFunctionFromModule(Module* module, const char* function)
    {
        void* addr = GetProcAddress(module->handle, function);
        if(! addr )
        {
            smol::Log::error("Falhou ao pegar o ponteiro para a funcao %s do module", function);
            return nullptr;
        }

        return addr;
    }

    const unsigned char* Platform::getKeyboardState()
    {
        return (const unsigned char*)&Window::keyboardState;
    }

    char* Platform::loadFileToBuffer(const char* fileName, size_t* loadedFileSize, size_t extraBytes, size_t offset)
    {
        FILE* fd = fopen(fileName, "rb");

        if (! fd)
        {
            smol::Log::error("Nao conseguiu abrir o aruqivo '%s'", fileName);
            return nullptr;
        }

        fseek(fd, 0, SEEK_END);
        size_t fileSize = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        const size_t totalBufferSize = fileSize + extraBytes;
        char* buffer = new char[totalBufferSize];

        if(! fread(buffer + offset, fileSize, 1, fd) )
        {
            smol::Log::error("Falhou ao ler do arquivo '%s'", fileName);            
            delete[] buffer;
            buffer = nullptr;
        }

        fclose(fd);

        return buffer;
    }

    void Platform::unloadFileBuffer(const char* fileBuffer)
    {
        delete[] fileBuffer;
    }
}
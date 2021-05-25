#include <smol/smol_version.h>
#include <smol/smol_platform.h>
#include "smol_resource_win64.h"
#include <smol/gl/glcorearb.h>
#include <smol/gl/wglext.h>
#include <cstdio>

namespace smol 
{
    PFNGLCLEARCOLORPROC glClearColor;
    PFNGLCLEARPROC glClear;

    const UINT SMOL_CLOSE_WINDOW = WM_USER + 1;

    struct Window 
    {
        HWND handle;
        bool shouldClose;
        HDC dc;
        HGLRC rc;
    };

    // Ref: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms633573(v=vs.85)
    LRESULT smolWindowProc(
        HWND   hwnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam
    ) 
    {
        switch(uMsg) 
        {
            case WM_CLOSE:

                // ref: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postmessagea
                PostMessage(hwnd, SMOL_CLOSE_WINDOW, 0, 0);

                break;

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

    /* 
     * ChoosePixelFormat  // Contexto OpenGL 1.1
     * SetPixelFormat()   // Associa o pixel format ao Device Context (DC)
     * wglCreateContext() // Cria um contexto OpenGL 1.1
     * wglMakeCurrent()   // Associa o contexto OpenGL (rc) ao nosso Device Context (DC)
     * 
     * wgGetProcAddress()  // Pegar o ponteiro pra wglChoosePixelFormatARB() e CreateContextAttribsARB()
     * 
     * https://www.khronos.org/registry/OpenGL/index_gl.php
     * https://www.khronos.org/registry/OpenGL/api/GL/glcorearb.h
     *
     */
    bool Platform::initOpenGL(Window *window, int glVersionMajor, int glVersionMinor, int colorBits, int depthBits)
    {
        Window* dummyWindow = createWindow(0, 0, "Dummy Window");

        // ref: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-choosepixelformat
        PIXELFORMATDESCRIPTOR pfd = { 
            sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
            1,                     // version number  
            PFD_DRAW_TO_WINDOW |   // support window  
            PFD_SUPPORT_OPENGL |   // support OpenGL  
            PFD_DOUBLEBUFFER,      // double buffered  
            PFD_TYPE_RGBA,         // RGBA type  
            (BYTE) depthBits,      // 24-bit color depth  
            0, 0, 0, 0, 0, 0,      // color bits ignored  
            0,                     // no alpha buffer  
            0,                     // shift bit ignored  
            0,                     // no accumulation buffer  
            0, 0, 0, 0,            // accum bits ignored  
            (BYTE) colorBits,      // 32-bit z-buffer      
            0,                     // no stencil buffer  
            0,                     // no auxiliary buffer  
            PFD_MAIN_PLANE,        // main layer  
            0,                     // reserved  
            0, 0, 0                // layer masks ignored  
        }; 

        int pixelFormat = ChoosePixelFormat(dummyWindow->dc, &pfd);

        if(! pixelFormat )
        {
            destroyWindow(dummyWindow);
            return false;
        }        

        if(! SetPixelFormat(dummyWindow->dc, pixelFormat, &pfd) ) 
        {
            destroyWindow(dummyWindow);
            return false;
        }

        // Ref: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglcreatecontext
        HGLRC rc = wglCreateContext(dummyWindow->dc);
        if (! rc )
        {
            destroyWindow(dummyWindow);
            return false;
        }

        // Ref: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglmakecurrent
        if (! wglMakeCurrent(dummyWindow->dc, rc) ) 
        {
            destroyWindow(dummyWindow);
            return false;  
        }

        // Ref: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglgetprocaddress
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
        
        wglMakeCurrent(0, 0);

        wglDeleteContext(rc);
        DeleteDC(dummyWindow->dc);
        DestroyWindow(dummyWindow->handle);
        destroyWindow(dummyWindow);

        int numPixelFormats = 0;

        const int pixelFormatAttribList[] = { 
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_COLOR_BITS_ARB, colorBits,
            WGL_DEPTH_BITS_ARB, depthBits,
            0
        };
        
        wglChoosePixelFormatARB(window->dc, pixelFormatAttribList, nullptr, 1, &pixelFormat, (UINT*) &numPixelFormats);

        if( numPixelFormats <= 0 )
        {
            return false;           
        }


        if(! SetPixelFormat(window->dc, pixelFormat, &pfd) )
        {
            return false;
        }

        const int contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, glVersionMajor,
            WGL_CONTEXT_MINOR_VERSION_ARB, glVersionMinor,
            WGL_CONTEXT_FLAGS_ARB,

            #if SMOL_DEBUG
            WGL_CONTEXT_DEBUG_BIT_ARB |
            #endif

            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        rc = wglCreateContextAttribsARB(window->dc, 0, contextAttribs);

        if(! rc )
        {
           return false;
        }

        window->rc = rc;

        if(! wglMakeCurrent(window->dc, window->rc) )
        {
            return false;
        }

        HMODULE opengl32dll = GetModuleHandleA("OpenGL32.dll");
        glClearColor = (PFNGLCLEARCOLORPROC) GetProcAddress(opengl32dll, "glClearColor");
        glClear = (PFNGLCLEARPROC) GetProcAddress(opengl32dll, "glClear");

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        return true;
    }


    Window* Platform::createWindow(int width, int height, const char* title)
    {

        const char* smolWindowClass = "SMOL_WINDOW_CLASS";

        // ref: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
        WNDCLASSEXA wc = {};

        HINSTANCE hInstance = GetModuleHandleA(NULL);

        // Ref: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclassinfoexa
        if(! GetClassInfoExA(hInstance, smolWindowClass, &wc) )
        {
            wc.cbSize = sizeof(WNDCLASSEXA);
            wc.style = CS_OWNDC;
            wc.lpfnWndProc = smolWindowProc;
            // wc.cbClsExtra;
            // wc.cbWndExtra;
            wc.hInstance = hInstance;
            wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCE(SMOL_ICON_ID));
            // wc.hCursor;
            wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
            // wc.lpszMenuName;
            wc.lpszClassName = smolWindowClass;
            // wc.hIconSm;

            if (! RegisterClassExA(&wc) )
            {
                return nullptr;
            }
        }
        // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
        HWND windowHandle = CreateWindowExA(
            0,
            smolWindowClass,
            title,

            // ref: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
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
            return nullptr;
        }

        Window* window = new Window;
        window->handle = windowHandle;
        window->dc = GetDC(windowHandle);
        window->shouldClose = false;

        return window;
    }

    void Platform::updateWindowEvents(Window* window)
    {

        MSG msg;
        HWND hwnd = window->handle;

        // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
        while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) 
        {
            if(msg.message == SMOL_CLOSE_WINDOW) 
            {

                window->shouldClose = true;

                return;

            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            glClear(GL_COLOR_BUFFER_BIT);
            SwapBuffers(window->dc);
        }
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
        delete window;
    }
}
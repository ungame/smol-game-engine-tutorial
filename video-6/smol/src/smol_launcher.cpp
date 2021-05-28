#include <smol/smol.h>
#include <smol/smol_platform.h>
#include <smol/smol_engine.h>
#include <smol/smol_gl.h>

namespace smol 
{

    namespace launcher 
    {

        int smolMain(int argc, char** argv)
        {
            smol::Engine engine;
            smol::Platform& platform = engine.platorm;

            if (! platform.initOpenGL(3, 1) ) {
                return 1;
            }

            smol::Window* window = platform.createWindow(800, 600, "Smol Engine");

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

            unsigned int vao, vbo, ibo;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            float vertices[] = {
                -0.5f, -0.5, 0.0f,
                 0.5f, -0.5, 0.0f,
                 0.5,   0.5, 0.0,
                -0.5,   0.5, 0.0
            };

            unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);

            // Ref: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            while(! platform.getWindowCloseFlag(window) ) 
            {
                glClear(GL_COLOR_BUFFER_BIT);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                platform.updateWindowEvents(window);
            }

            platform.destroyWindow(window);

            return 0;
        }

    }
}

#ifdef SMOL_PLATFORM_WINDOWS
#include "win64\smol_resource_win64.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return smol::launcher::smolMain(0, (char**) lpCmdLine);
}

#endif

int main(int argc, char** argv) {
    return smol::launcher::smolMain(argc, argv);
}
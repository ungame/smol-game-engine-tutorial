#include <smol/smol.h>
#include <smol/smol_game.h>
#include <smol/smol_platform.h>
#include <smol/smol_engine.h>
#include <smol/smol_gl.h>
#include <smol/smol_gl.h>

namespace smol 
{
    const char* vertexSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 vertPos;\n"
    "layout (location = 1) in vec3 vertColorIn;\n"
    "out vec4 vertColor;\n"
    "void main() {\n"
    " gl_Position = vec4(vertPos, 1.0);\n"
    " vertColor = vec4(vertColorIn, 1.0);\n"
    "}";


    const char* fragmentSource = 
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "in vec4 vertColor;\n"
    "void main() {\n"
    " fragColor = vertColor;\n"
    "\n}";

    typedef GLuint Shader;

    Shader loadShader(const char* vertexSource, const char* fragSource) 
    {
        GLint status;
        const int errorLogSize = 1024;
        GLsizei errorBufferLen = 0;
        char errorBuffer[errorLogSize];

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vertexSource, 0);
        glCompileShader(vShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);

        if (! status )
        {
            glGetShaderInfoLog(vShader, errorLogSize, &errorBufferLen, errorBuffer);
            LogInfo("Compiling VERTEX SHADER: %s\n", errorBuffer);
            glDeleteShader(vShader);
            return 0;
        }

        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fragSource, 0);
        glCompileShader(fShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);

        if (! status )
        {
            glGetShaderInfoLog(fShader, errorLogSize, &errorBufferLen, errorBuffer);
            LogInfo("Compiling FRAGMENT SHADER: %s\n", errorBuffer);
            glDeleteShader(fShader);
            glDeleteShader(vShader);
            return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (! status )
        {
            glGetProgramInfoLog(program, errorLogSize, &errorBufferLen, errorBuffer);
            LogInfo("Linking SHADER : %s\n", errorBuffer);

            glDeleteShader(fShader);
            glDeleteShader(vShader);
            glDeleteProgram(program);
            return 0;
        }

        glDeleteShader(fShader);
        glDeleteShader(vShader);

        return program;
    }


    namespace launcher 
    {

        int smolMain(int argc, char** argv)
        {
            if (! Platform::initOpenGL(3, 1) ) 
            {
                return 1;
            }

            smol::Window* window = Platform::createWindow(800, 600, "Smol Engine");

            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

            unsigned int vao, vbo, ibo;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            float vertices[] = {
                -0.5f, -0.5, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, -0.5, 0.0f, 0.0f, 1.0f, 0.0f,
                 0.5,   0.5, 0.0,  0.0f, 0.0f, 1.0f, 
                -0.5,   0.5, 0.0,  0.0f, 0.5f, 0.5f
            };

            unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
            
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // load shader
            Shader shader = loadShader(vertexSource, fragmentSource);
            glUseProgram(shader);

            smol::Module* game = Platform::loadModule("game.dll");
            SMOL_GAME_CALLBACK_ONSTART onGameStartCallback = (SMOL_GAME_CALLBACK_ONSTART) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONSTART);

            SMOL_GAME_CALLBACK_ONSTOP onGameStopCallback = (SMOL_GAME_CALLBACK_ONSTOP) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONSTOP);

            SMOL_GAME_CALLBACK_ONUPDATE onGameUpdateCallback = (SMOL_GAME_CALLBACK_ONUPDATE) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONUPDATE);

            onGameStartCallback();

            while(! Platform::getWindowCloseFlag(window) ) 
            {

                onGameUpdateCallback(0.0f);

                glClear(GL_COLOR_BUFFER_BIT);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                Platform::updateWindowEvents(window);
            }

            onGameStopCallback();
            Platform::unloadModule(game);

            Platform::destroyWindow(window);
 
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

int main(int argc, char** argv) 
{
    return smol::launcher::smolMain(argc, argv);
}
#include <smol/smol.h>
#include <smol/smol_game.h>
#include <smol/smol_platform.h>
#include <smol/smol_engine.h>
#include <smol/smol_keyboard.h>
#include <smol/smol_assetmanager.h>
#include <smol/smol_log.h>
#include <smol/smol_renderer.h>

#if defined(SMOL_DEBUG)
    #define SMOL_LOGFILE nullptr
    #define SMOL_LOGLEVEL smol::Log::LogType::LOG_ALL
#else
    #define SMOL_LOGFILE "smol_engine_log.txt"
    #define SMOL_LOGLEVEL smol::Log::LogType::LOG_FATAL |  smol::Log::LOG_ERROR
#endif

#ifndef SMOL_GAME_MODULE_NAME
    #ifdef SMOL_PLATFORM_WINDOWS
        #define SMOL_GAME_MODULE_NAME "game.dll"
    #else
        #define SMOL_GAME_MODULE_NAME "game.so"
    #endif
#endif

namespace smol 
{
    namespace launcher 
    {

        int smolMain(int argc, char** argv)
        {
            Log::verbosity(SMOL_LOGLEVEL);

            if (! Platform::initOpenGL(3, 1) ) 
            {
                return 1;
            }

            smol::Module* game = Platform::loadModule("game.dll");
            SMOL_GAME_CALLBACK_ONSTART onGameStartCallback = (SMOL_GAME_CALLBACK_ONSTART) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONSTART);

            SMOL_GAME_CALLBACK_ONSTOP onGameStopCallback = (SMOL_GAME_CALLBACK_ONSTOP) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONSTOP);

            SMOL_GAME_CALLBACK_ONUPDATE onGameUpdateCallback = (SMOL_GAME_CALLBACK_ONUPDATE) Platform::getFunctionFromModule(game, SMOL_CALLBACK_NAME_ONUPDATE);

            if (! (game && onGameStartCallback && onGameStopCallback && onGameUpdateCallback))
            {
                smol::Log::error("Falhou ao carregar o modulo do jogo");
                return 1;
            }

            onGameStartCallback();

            const int WIDTH = 1024;
            const int HEIGHT = 576;
            smol::Window* window = Platform::createWindow(WIDTH, HEIGHT, "Smol Engine");


            // Texturing

            // Create a procedural checker texture
#if 0
            const int texWidth = 1024;
            const int texHeight = texWidth;
            const int squareCount = 64;
            const int squareSize = texWidth / squareCount;
            unsigned char* texData = new unsigned char[texWidth * texHeight * 3]; 
            unsigned char* pixel = texData;

            for(int i = 0; i < texWidth; i++) 
            {
                for(int j = 0; j < texHeight; j++)
                {
                    int x = i / squareSize;
                    int y = j / squareSize;
                    int squareNumber = x * squareCount + y;

                    unsigned char color;
                    bool isOdd = (squareNumber & 1);
                    if(x & 1)
                    {
                        color = (isOdd) ? 0xAA : 0x55; 
                    }
                    else
                    {
                        color = (isOdd) ? 0x55 : 0xAA;
                    }

                    *pixel++ = color;
                    *pixel++ = color;
                    *pixel++ = color;
                }
            }
#endif
  
            bool isOrtho = false;
            float scale = 1.0f;
            float rotation = 0.0f;
            float x = 0.0f;
            bool mipmap = false;

            Renderer::init(WIDTH, HEIGHT);
            while(! Platform::getWindowCloseFlag(window) ) 
            {
                bool update = false;

                onGameUpdateCallback(0.0f);


                // if( smol::Keyboard::getKeyDown(smol::KEYCODE_SPACE) )
                // {
                //     smol::Log::info(mipmap ? "MIPMAP_LINEAR" : "LINEAR");
                //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
                //     mipmap = !mipmap;
                // }

                // if( smol::Keyboard::getKeyDown(smol::KEYCODE_P) )
                // {
                //     isOrtho = !isOrtho;
                //     update = true;
                // }

                // if( smol::Keyboard::getKey(smol::KEYCODE_S) )
                // {
                //     if( smol::Keyboard::getKey(smol::KEYCODE_SHIFT) )
                //     {
                //         scale -= 0.02f;
                //     } else {
                //         scale += 0.02f;
                //     }
                //     update = true;
                // }

                // if( smol::Keyboard::getKey(smol::KEYCODE_R) )
                // {
                //     if( smol::Keyboard::getKey(smol::KEYCODE_SHIFT) )
                //     {
                //         rotation -= 0.02f;
                //     } else {
                //         rotation += 0.02f;
                //     }
                //     update = true;
                // }

                // if( smol::Keyboard::getKey(smol::KEYCODE_T) )
                // {
                //     if( smol::Keyboard::getKey(smol::KEYCODE_SHIFT) )
                //     {
                //         x -= 0.02f;
                //     } else {
                //         x += 0.02f;
                //     }
                //     update = true;
                // }

                // if(update)
                // {
                //     // choose projection
                //     Mat4 m = isOrtho ? orthographic : perspective;

                //     // position
                //     Mat4 translationMatrix = Mat4::initTranslation(x, 0.0f, 0.0f);
                //     Mat4 transformed = Mat4::mul(m, translationMatrix);
               
                //     // scale
                //     Mat4 scaleMatrix = Mat4::initScale(scale);
                //     transformed = Mat4::mul(transformed, scaleMatrix);

                //     // rotation
                //     Mat4 rotationMatrix = Mat4::initRotation(0.0f, 1.0f, 0.0f, rotation);
                //     transformed = Mat4::mul(transformed, rotationMatrix);

                //     // update shader
                //     glUniformMatrix4fv(uniform, 1, 0, (const float*) transformed.e);
                // }

                Platform::updateWindowEvents(window);
                Renderer::render();
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
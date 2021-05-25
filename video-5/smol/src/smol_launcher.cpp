#include <smol/smol.h>
#include <smol/smol_platform.h>
#include <smol/smol_engine.h>

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



            while(! platform.getWindowCloseFlag(window) ) 
            {

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
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
            engine.platorm.showMessage("Hwllo SMOL engine!");
            return 0;
        }

    }
}

#ifdef SMOL_PLATFORM_WINDOWS
#include "win64\smol_resource_win64.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    LoadIcon(hInstance, MAKEINTRESOURCE(SMOL_ICON_ID));
    return smol::launcher::smolMain(0, (char**) lpCmdLine);
}

#endif
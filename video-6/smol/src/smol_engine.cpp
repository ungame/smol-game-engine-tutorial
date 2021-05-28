#include <smol/smol_engine.h>
#include <stdio.h>

namespace smol 
{

    Engine::Engine() 
    {
        printf("Engine started\n");
    }

    Engine::~Engine() 
    {
        printf("Engine terminated\n");
    }

}
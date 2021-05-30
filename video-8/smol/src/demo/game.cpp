#include <smol/smol.h>
#include <smol/smol_game.h>

void onStart() 
{
    LogInfo("Game Started!");
}

void onUpdate(float deltaTime)
{
    LogInfo("Game Updated...");
}

void onStop() 
{
    LogInfo("Game Stopped!\n");
}
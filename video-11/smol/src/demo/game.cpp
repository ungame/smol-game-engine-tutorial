#include <smol/smol.h>
#include <smol/smol_game.h>
#include <smol/smol_keyboard.h>

void onStart() 
{
    LogInfo("Game Started!");
}

void onUpdate(float deltaTime)
{
    // LogInfo("Game Updated...");
   if( smol::Keyboard::getKeyDown(smol::KEYCODE_J) )
   {
       LogInfo("PRESSED THIS FRAME");
   }
   else if( smol::Keyboard::getKeyUp(smol::KEYCODE_J) )
   {
       LogInfo("RELEASED THIS FRAME");
   }
    else if( smol::Keyboard::getKey(smol::KEYCODE_J) )
   {
       LogInfo("HOLDING...");
   }
}

void onStop() 
{
    LogInfo("Game Stopped!\n");
}
#include <smol/smol.h>
#include <smol/smol_game.h>
#include <smol/smol_keyboard.h>
#include <smol/smol_log.h>
#include <stdio.h>

void onStart() 
{
    smol::Log::info("Game Started!");
}

void onUpdate(float deltaTime)
{
   if( smol::Keyboard::getKeyDown(smol::KEYCODE_J) )
   {
       smol::Log::info("PRESSED THIS FRAME");
   }
   else if( smol::Keyboard::getKeyUp(smol::KEYCODE_J) )
   {
       smol::Log::info("RELEASED THIS FRAME");
   }
    else if( smol::Keyboard::getKey(smol::KEYCODE_J) )
   {
       smol::Log::info("HOLDING...");
   }
}

void onStop() 
{
    smol::Log::info("Game Stopped!\n");
}
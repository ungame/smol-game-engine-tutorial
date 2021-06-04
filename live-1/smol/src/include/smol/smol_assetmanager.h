#ifndef SMOL_ASSETMANAGER_H
#define SMOL_ASSETMANAGER_H

#include <smol/smol_engine.h>

namespace smol 
{
    struct Image
    {
        int width;
        int height;
        int bitsPerPixel;
        char* data;
    };

    struct SMOL_ENGINE_API AssetManager
    {
        static Image* loadImageBitmap(const char* fileName);
        static void unloadImageBitmap(Image* image);
    };
}

#endif
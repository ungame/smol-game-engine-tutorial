#ifndef SMOL_RENDERER_H
#define SMOL_RENDERER_H

#include <smol/smol_engine.h>

namespace smol
{
    struct SMOL_ENGINE_API Renderer
    {
        static void init(int width, int height);
        static void render();
    };
}

#endif
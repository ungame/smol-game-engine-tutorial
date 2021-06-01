#ifndef SMOL_MAT4
#define SMOL_MAT4

#include <smol/smol_engine.h>

namespace smol
{
    struct SMOL_ENGINE_API Mat4
    {
        float e[4][4];

        static Mat4 identity();
        static Mat4 initPerspective(float fov, float aspect, float zNear, float zFar);
        static Mat4 initOrtho(float left, float right, float top, float bottom, float zNear, float zFar);
        static Mat4 mul(Mat4& a, Mat4& b);
        static Mat4 initScale(float factor);
        static Mat4 initScale(float x, float y, float z);
        static Mat4 initTranslation(float x, float y, float z);
        static Mat4 initRotation(float x, float y, float z, float angle);
    };
}

#endif
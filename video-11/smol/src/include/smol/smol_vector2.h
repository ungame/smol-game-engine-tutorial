#ifndef SMOL_VECTOR2
#define SMOL_VECTOR2

namespace smol
{
    struct Vector2
    {
        float x;
        float y;

        Vector2(float x, float y);
        Vector2();

        // Vector/Scalar operations
        Vector2& sum(float f);
        Vector2& sub(float f);
        Vector2& mult(float f);
        Vector2& div(float f);
        // Vector/Vector operations
        Vector2& sum(Vector2& other);
        Vector2& sub(Vector2& other);
        Vector2& mult(Vector2& other);
        Vector2& div(Vector2& other);

        float length();
        Vector2& normalized();
        Vector2& absolute();

        float dot(Vector2& other);
        float cross(Vector2& other);

        void set(float x, float y);
    };
}

#endif
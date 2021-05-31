#ifndef SMOL_VECTOR3
#define SMOL_VECTOR3

namespace smol
{
    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z);
        Vector3();

        // Vector/Scalar operations
        Vector3& sum(float f);
        Vector3& sub(float f);
        Vector3& mult(float f);
        Vector3& div(float f);
        // Vector/Vector operations
        Vector3& sum(Vector3& other);
        Vector3& sub(Vector3& other);
        Vector3& mult(Vector3& other);
        Vector3& div(Vector3& other);

        float length();
        Vector3& normalized();
        Vector3& absolute();

        float dot(Vector3& other);
        Vector3 cross(Vector3& other);

        void set(float x, float y, float z);
    };
}

#endif
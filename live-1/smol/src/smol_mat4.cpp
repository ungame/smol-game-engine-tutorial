#include <smol/smol.h>
#include <smol/smol_mat4.h>
#include <smol/smol_log.h>
#include <math.h>

namespace smol
{
    Mat4 Mat4::identity()
    {
        Mat4 m = {0};
        
        m.e[0][0] = m.e[1][1] = m.e[2][2] = m.e[3][3] = 1.0f;
        
        return m;
    }

    
    Mat4 Mat4::initPerspective(float fov, float aspect, float zNear, float zFar)
    {
        SMOL_ASSERT(zNear >= 0.0f, "zNear deve ser maior ou igual a 0.0");
        SMOL_ASSERT(zFar > 0.0f, "zFar dever ser maior que 0.0");
        SMOL_ASSERT(zFar > zNear, "zFar deve ser maior que zNear.");

        const float tanHalfFov = (float) tan(fov/2); 
        Mat4 result = Mat4::identity();

        result.e[0][0] = 1 / (aspect * tanHalfFov);
        result.e[1][1] = 1 / tanHalfFov;
        result.e[2][2] = - (zFar + zNear) / (zFar - zNear);
        result.e[2][3] = -1;
        result.e[3][2] = - (2 * zFar * zNear) / (zFar - zNear);

        return result;
    }

    Mat4 Mat4::initOrtho(float left, float right, float top, float bottom, float zNear, float zFar)
    {
        Mat4 result = Mat4::identity();

        result.e[0][0] = 2 / (right - left);
        result.e[1][1] = 2 / (top - bottom);
        result.e[2][2] = - 2 / (zFar - zNear);
        result.e[3][0] = - (right + left) / (right - left);
        result.e[3][1] = - (top + bottom) / (top - bottom);
        result.e[3][2] = - (zFar + zNear) / (zFar - zNear);
        result.e[3][3] = 1;

        return result;
    }

    Mat4 Mat4::initScale(float factor)
    {
        return Mat4::initScale(factor, factor, factor);
    }

    Mat4 Mat4::initScale(float x, float y, float z)
    {
        Mat4 m = {0};
        
        m.e[0][0] = x;
        m.e[1][1] = y;
        m.e[2][2] = z;
        m.e[3][3] = 1.0f;
        
        return m;
    }

    Mat4 Mat4::initTranslation(float x, float y, float z)
    {
        Mat4 m = Mat4::identity();
        
        m.e[3][0] = x;
        m.e[3][1] = y;
        m.e[3][2] = z;
        
        return m;  
    }

    Mat4 Mat4::mul(Mat4& a, Mat4& b)
    {
        Mat4 m;

        for(int line = 0; line < 4; line++)
        {
            for(int column = 0; column < 4; column++)
            {
                m.e[column][line] = a.e[0][line] * b.e[column][0] + 
                                    a.e[1][line] * b.e[column][1] + 
                                    a.e[2][line] * b.e[column][2] + 
                                    a.e[3][line] * b.e[column][3]; 
            }
        }

        return m;
    }

    // Ref: https://en.wikipedia.org/wiki/Rotation_matrix
    Mat4 Mat4::initRotation(float x, float y, float z, float angle) 
    {
        const float c = (float) cos(angle);
        const float s = (float) sin(angle);
        const float oneMinusCos = 1 - c;

        Mat4 m = Mat4::identity();

        m.e[0][0] = x * x * oneMinusCos + c;
        m.e[1][0] = x * y * oneMinusCos + z * s;
        m.e[2][0] = x * z * oneMinusCos - y * s;

        m.e[0][1] = y * x * oneMinusCos - z * s;
        m.e[1][1] = y * y * oneMinusCos + c;
        m.e[2][1] = y * x * oneMinusCos + x * s;

        m.e[0][2] = z * x * oneMinusCos + y * s;
        m.e[1][2] = z * y * oneMinusCos - x * s;
        m.e[2][2] = z * z * oneMinusCos + c;

        return m; 
    }
}

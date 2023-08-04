#include "calc.h"
#include <array>
#include <cmath>

const float pi = 3.14159265358979323846f;

// m.mat4x4(identity)
std::array<std::array<float, 4>, 4> mat4x4(bool identity) {
    float n = (identity) ? 1.0f : 0.0f;
    // Create our 4x4 matrix w/ N = 1:0
    std::array<std::array<float, 4>, 4> temp = { {
        {n, 0.0, 0.0, 0.0},
        {0.0, n, 0.0, 0.0},
        {0.0, 0.0, n, 0.0},
        {0.0, 0.0, 0.0, n}
    } };

    return temp;
}
// m.vec4(x, y, z, w)
std::array<float, 4> vec4(float x, float y, float z, float w) {
    // Create and return the 1D array directly
    return std::array<float, 4>{x, y, z, w};
}
// m.mul(m1, m2) 4x4 matrix multiplication
std::array<std::array<float, 4>, 4> mul(const std::array<std::array<float, 4>, 4>& m1, const std::array<std::array<float, 4>, 4>& m2) {
    std::array<std::array<float, 4>, 4> temp = { {
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0}
    } };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                temp[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return temp;
}
// m.mul(m1, m2) 4x1 matrix-vector multiplication
std::array<float, 4> mul(const std::array<std::array<float, 4>, 4>& m1, const std::array<float, 4>& v1) {
    std::array<float, 4> temp = { 0.0 };

    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            temp[i] += m1[i][k] * v1[k];
        }
    }

    return temp;
}
// m.translate(x, y, z) vector translation
std::array<std::array<float, 4>, 4> translate(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> temp = mat4x4(true);

    temp[0][3] = x;
    temp[1][3] = y;
    temp[2][3] = z;

    return temp;
}
// m.rotationX(angle)
std::array<std::array<float, 4>, 4> rotationX(float angle) {
    float radAngle = angle * pi / 180;
    std::array<std::array<float, 4>, 4> temp = mat4x4(true);
    temp[1][1] = std::cos(radAngle);
    temp[2][1] = -std::sin(radAngle);
    temp[1][2] = std::sin(radAngle);
    temp[2][2] = std::cos(radAngle);
    return temp;
}
// m.rotationY(angle)
std::array<std::array<float, 4>, 4> rotationY(float angle) {
    float radAngle = angle * pi / 180;
    std::array<std::array<float, 4>, 4> temp = mat4x4(true);
    temp[0][0] = std::cos(radAngle);
    temp[2][0] = std::sin(radAngle);
    temp[0][2] = -std::sin(radAngle);
    temp[2][2] = std::cos(radAngle);

    return temp;
}
// m.rotationZ(angle)
std::array<std::array<float, 4>, 4> rotationZ(float angle) {
    float radAngle = angle * pi / 180;
    std::array<std::array<float, 4>, 4> temp = mat4x4(true);
    temp[0][0] = std::cos(radAngle);
    temp[1][0] = -std::sin(radAngle);
    temp[0][1] = std::sin(radAngle);
    temp[1][1] = std::cos(radAngle);

    return temp;
}
// m.rotateXYZ(x, y, z)
std::array<std::array<float, 4>, 4> rotateXYZ(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> rX = rotationX(x);
    std::array<std::array<float, 4>, 4> rY = rotationY(y);
    std::array<std::array<float, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<float, 4>, 4> f = mul(rZ, rY);
    return mul(f, rX);
}
// m.rotateYXZ(x, y, z)
std::array<std::array<float, 4>, 4> rotateYXZ(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> rX = rotationX(x);
    std::array<std::array<float, 4>, 4> rY = rotationY(y);
    std::array<std::array<float, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<float, 4>, 4> f = mul(rZ, rX);
    return mul(f, rY);
}
// m.rotateYZX(x, y, z)
std::array<std::array<float, 4>, 4> rotateYZX(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> rX = rotationX(x);
    std::array<std::array<float, 4>, 4> rY = rotationY(y);
    std::array<std::array<float, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<float, 4>, 4> f = mul(rX, rZ);
    return mul(f, rY);
}
// m.rotateZXY(x, y, z)
std::array<std::array<float, 4>, 4> rotateZXY(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> rX = rotationX(x);
    std::array<std::array<float, 4>, 4> rY = rotationY(y);
    std::array<std::array<float, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<float, 4>, 4> f = mul(rY, rX);
    return mul(f, rZ);
}
// m.rotateZYX(x, y, z)
std::array<std::array<float, 4>, 4> rotateZYX(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> rX = rotationX(x);
    std::array<std::array<float, 4>, 4> rY = rotationY(y);
    std::array<std::array<float, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<float, 4>, 4> f = mul(rX, rY);
    return mul(f, rZ);
}
// m.scale(x, y, z) 
std::array<std::array<float, 4>, 4> scale(float x, float y, float z) {
    std::array<std::array<float, 4>, 4> temp = mat4x4(true);

    temp[0][0] = x;
    temp[1][1] = y;
    temp[2][2] = z;

    return temp;
}
// m.perspective(sizeX, sizeY, nearPlane, far, FOV)
std::array<std::array<float, 4>, 4> perspective(float sizeX, float sizeY, float nearPlane, float farPlane, float FOV) {
    float aspect = sizeX / sizeY;

    float top = nearPlane * std::tan((FOV * pi / 180)/2);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    std::array<std::array<float, 4>, 4> temp = mat4x4();

    temp[0] = { (2 * nearPlane) / (right - left), 0.0, (right + left) / (right - left), 0.0 };
    temp[1] = { 0.0, (2 * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0 };
    temp[2] = { 0.0, 0.0, -(farPlane + nearPlane) / (farPlane - nearPlane), -(2 * farPlane * nearPlane) / (farPlane - nearPlane) };
    temp[3] = { 0.0, 0.0, -1.0, 0.0 };

    return temp;
}
// m.divW(vec)
std::array<float, 4> divW(const std::array<float, 4>& vec) {
    std::array<float, 4> temp = std::array<float, 4>{0.0, 0.0, 0.0, 1.0};
    temp[0] = vec[0] / vec[3];
    temp[1] = vec[1] / vec[3];
    temp[2] = vec[2] / vec[3];
    return temp;
}
// m.cross4(x1, y1, x2, y2, x3, y3, x4, y4)
float cross4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}
// m.cross2(v1, v2)
std::array<float, 3> cross2(const std::array<float, 3>& v1, const std::array<float, 3>& v2) {
    float x = v1[1] * v2[2] - v1[2] * v2[1];
    float y = v1[2] * v2[0] - v1[0] * v2[2];
    float z = v1[0] * v2[1] - v1[1] * v2[0];

    return { x, y, z };
}
// m.sub(v1, v2)
std::array<std::array<float, 3>, 3> sub(const std::array<std::array<float, 3>, 3>& v1, const std::array<std::array<float, 3>, 3>& v2) {
    std::array<std::array<float, 3>, 3> result{};
    int n = std::min(v1.size(), v2.size()); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < v1[i].size(); ++j) {
            result[i][j] = v1[i][j] - v2[i][j];
        }
    }

    return result;
}
// m.normalize(v)
std::array<float, 3> normalize(const std::array<float, 3>& v) {
    float magnitudeSquared = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    float inverseMagnitude = 1.0f / std::sqrt(magnitudeSquared);

    return { v[0] * inverseMagnitude, v[1] * inverseMagnitude, v[2] * inverseMagnitude };
}
// m.dot(v1, v2)
float dot(const std::array<float, 4>& v1, const std::array<float, 4>& v2) {
    float dotProduct = 0.0;
    for (int i = 0; i < 4; ++i) {
        dotProduct += v1[i] * v2[i];
    }
    return dotProduct;
}
// m.length(v)
float length(const std::array<float, 4>& v) {
    float magnitudeSquared = 0.0;
    for (int i = 0; i < 4; ++i) {
        magnitudeSquared += v[i] * v[i];
    }
    return std::sqrt(magnitudeSquared);
}
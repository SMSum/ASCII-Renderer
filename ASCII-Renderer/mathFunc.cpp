#include "mathFunc.h"
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
            temp[i][j] = m1[i][0] * m2[0][j] +
                m1[i][1] * m2[1][j] +
                m1[i][2] * m2[2][j] +
                m1[i][3] * m2[3][j];
        }
    }

    return temp;
}
// m.mul(m1, m2) 4x1 matrix-vector multiplication
std::array<float, 4> mul(const std::array<std::array<float, 4>, 4>& m1, const std::array<float, 4>& v1) {
    const float* matrix0 = m1[0].data();
    const float* matrix1 = m1[1].data();
    const float* matrix2 = m1[2].data();
    const float* matrix3 = m1[3].data();

    std::array<float, 4> temp = {
        matrix0[0] * v1[0] + matrix0[1] * v1[1] + matrix0[2] * v1[2] + matrix0[3] * v1[3],
        matrix1[0] * v1[0] + matrix1[1] * v1[1] + matrix1[2] * v1[2] + matrix1[3] * v1[3],
        matrix2[0] * v1[0] + matrix2[1] * v1[1] + matrix2[2] * v1[2] + matrix2[3] * v1[3],
        matrix3[0] * v1[0] + matrix3[1] * v1[1] + matrix3[2] * v1[2] + matrix3[3] * v1[3]
    };

    return temp;
}
// m.mul(m1, v1) 4x4 x vec
Vertex mul(const std::array<std::array<float, 4>, 4>& matrix, const Vertex& vertex) {
    const float* matrix0 = matrix[0].data();
    const float* matrix1 = matrix[1].data();
    const float* matrix2 = matrix[2].data();
    const float* matrix3 = matrix[3].data();

    float x = matrix0[0] * vertex.x + matrix0[1] * vertex.y + matrix0[2] * vertex.z + matrix0[3];
    float y = matrix1[0] * vertex.x + matrix1[1] * vertex.y + matrix1[2] * vertex.z + matrix1[3];
    float z = matrix2[0] * vertex.x + matrix2[1] * vertex.y + matrix2[2] * vertex.z + matrix2[3];
    float w = matrix3[0] * vertex.x + matrix3[1] * vertex.y + matrix3[2] * vertex.z + matrix3[3];

    return { x, y, z, w };
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

    float top = nearPlane * std::tan((FOV * pi / 180) / 2);
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
// m.cross(v)
Vertex cross(const Vertex& a, const Vertex& b) {
    Vertex result{};
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
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
Vertex normalize(const Vertex& v) {
    float magnitudeSquared = v.x * v.x + v.y * v.y + v.z * v.z;
    float inverseMagnitude = 1.0f / std::sqrt(magnitudeSquared);

    return { v.x * inverseMagnitude, v.y * inverseMagnitude, v.z * inverseMagnitude };
}
// m.dot(v1, v2)
float dot(const Vertex& a, const Vertex& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

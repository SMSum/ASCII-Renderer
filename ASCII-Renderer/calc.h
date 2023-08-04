#ifndef CALC_H
#define CALC_H

#include <array>

// m.mat4x4
std::array<std::array<float, 4>, 4> mat4x4(bool identity = false);
// m.vec4
std::array<float, 4> vec4(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);
// m.mul 4x4 matrix multiplication
std::array<std::array<float, 4>, 4> mul(const std::array<std::array<float, 4>, 4>& m1, const std::array<std::array<float, 4>, 4>& m2);
// m.mul 4x1 matrix-vector multiplication
std::array<float, 4> mul(const std::array<std::array<float, 4>, 4>& m1, const std::array<float, 4>& v1);
// m.translate
std::array<std::array<float, 4>, 4> translate(float x = 0.0, float y = 0.0, float z = 0.0);
// m.rotationX
std::array<std::array<float, 4>, 4> rotationX(float angle);
// m.rotationY
std::array<std::array<float, 4>, 4> rotationY(float angle);
// m.rotationZ
std::array<std::array<float, 4>, 4> rotationZ(float angle);
// m.rotateXYZ
std::array<std::array<float, 4>, 4> rotateXYZ(float x, float y, float z);
// m.rotateYXZ
std::array<std::array<float, 4>, 4> rotateYXZ(float x, float y, float z);
// m.rotateYZX
std::array<std::array<float, 4>, 4> rotateYZX(float x, float y, float z);
// m.rotateZXY
std::array<std::array<float, 4>, 4> rotateZXY(float x, float y, float z);
// m.rotateZYX
std::array<std::array<float, 4>, 4> rotateZYX(float x, float y, float z);
// m.scale
std::array<std::array<float, 4>, 4> scale(float x, float y, float z);
// m.perspective
std::array<std::array<float, 4>, 4> perspective(float sizeX, float sizeY, float nearPlane, float farPlane, float FOV);
// m.divW
std::array<float, 4> divW(const std::array<float, 4>& vec);
// m.cross4
float cross4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
// m.cross2
std::array<float, 3> cross2(const std::array<float, 3>& v1, const std::array<float, 3>& v2);
// m.sub
std::array<std::array<float, 3>, 3> sub(const std::array<std::array<float, 3>, 3>& v1, const std::array<std::array<float, 3>, 3>& v2);
// m.normalize
std::array<float, 3> normalize(const std::array<float, 3>& v);
// m.dot(v1, v2)
float dot(const std::array<float, 4>& v1, const std::array<float, 4>& v2);
// m.length
float length(const std::array<float, 4>& v);

#endif
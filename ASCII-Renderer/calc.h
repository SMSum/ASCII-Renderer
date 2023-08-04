#ifndef CALC_H
#define CALC_H

#include <array>

// m.mat4x4
std::array<std::array<double, 4>, 4> mat4x4(bool identity = false);
// m.vec4
std::array<double, 4> vec4(double x = 0.0, double y = 0.0, double z = 0.0, double w = 1.0);
// m.mul 4x4 matrix multiplication
std::array<std::array<double, 4>, 4> mul(const std::array<std::array<double, 4>, 4>& m1, const std::array<std::array<double, 4>, 4>& m2);
// m.mul 4x1 matrix-vector multiplication
std::array<double, 4> mul(const std::array<std::array<double, 4>, 4>& m1, const std::array<double, 4>& v1);
// m.translate
std::array<std::array<double, 4>, 4> translate(double x = 0.0, double y = 0.0, double z = 0.0);
// m.rotationX
std::array<std::array<double, 4>, 4> rotationX(double angle);
// m.rotationY
std::array<std::array<double, 4>, 4> rotationY(double angle);
// m.rotationZ
std::array<std::array<double, 4>, 4> rotationZ(double angle);
// m.rotateXYZ
std::array<std::array<double, 4>, 4> rotateXYZ(double x, double y, double z);
// m.rotateYXZ
std::array<std::array<double, 4>, 4> rotateYXZ(double x, double y, double z);
// m.rotateYZX
std::array<std::array<double, 4>, 4> rotateYZX(double x, double y, double z);
// m.rotateZXY
std::array<std::array<double, 4>, 4> rotateZXY(double x, double y, double z);
// m.rotateZYX
std::array<std::array<double, 4>, 4> rotateZYX(double x, double y, double z);
// m.scale
std::array<std::array<double, 4>, 4> scale(double x, double y, double z);
// m.perspective
std::array<std::array<double, 4>, 4> perspective(double sizeX, double sizeY, double nearPlane, double farPlane, double FOV);
// m.divW
std::array<double, 4> divW(const std::array<double, 4>& vec);
// m.cross4
double cross4(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
// m.cross2
std::array<double, 3> cross2(const std::array<double, 3>& v1, const std::array<double, 3>& v2);
// m.sub
std::array<std::array<double, 3>, 3> sub(const std::array<std::array<double, 3>, 3>& v1, const std::array<std::array<double, 3>, 3>& v2);
// m.normalize
std::array<double, 3> normalize(const std::array<double, 3>& v);
// m.dot(v1, v2)
double dot(const std::array<double, 4>& v1, const std::array<double, 4>& v2);
// m.length
double length(const std::array<double, 4>& v);

#endif
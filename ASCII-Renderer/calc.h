#ifndef CALC_H
#define CALC_H

#include <array>

//m.mat4x4
std::array<std::array<double, 4>, 4> mat4x4(bool identity = false);
//m.vec4
std::array<double, 4> vec4(double x, double y, double z, double w = 1.0);
//m.mul
std::array<std::array<double, 4>, 4> mul(const std::array<std::array<double, 4>, 4>& m1,
    const std::array<std::array<double, 4>, 4>& m2);

#endif
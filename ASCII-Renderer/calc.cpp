#include "calc.h"
#include <array>


// m.mat4x4(identity)
std::array<std::array<double, 4>, 4> mat4x4(bool identity) {
    double n = (identity) ? 1.0 : 0.0;
    // Create our 4x4 matrix w/ N = 1:0
    std::array<std::array<double, 4>, 4> temp = { {
        {n, 0.0, 0.0, 0.0},
        {0.0, n, 0.0, 0.0},
        {0.0, 0.0, n, 0.0},
        {0.0, 0.0, 0.0, n}
    } };

    return temp;
}
// m.vec4(x, y, z, w)
std::array<double, 4> vec4(double x, double y, double z, double w) {
    // Create and return the 1D array directly
    return std::array<double, 4>{x, y, z, w};
}
// m.mul(m1, m2) matrix multiplication
std::array<std::array<double, 4>, 4> mul(const std::array<std::array<double, 4>, 4>& m1,
    const std::array<std::array<double, 4>, 4>& m2) {
    int r1 = m1.size();
    int r2 = m2.size();
    int c1 = m1[0].size();
    int c2 = m2[0].size();

    std::array<std::array<double, 4>, 4> temp = mat4x4();

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                temp[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    if (c2 == 4) {
        return temp;
    }
    else {
        return { {
            {temp[0][0], temp[1][0], temp[2][0], temp[3][0]}
        } };
    }
}
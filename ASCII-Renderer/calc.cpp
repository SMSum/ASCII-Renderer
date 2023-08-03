#include "calc.h"
#include <array>
#include <cmath>

const double pi = 3.14159265358979323846;

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
std::array<std::array<double, 4>, 4> mul(const std::array<std::array<double, 4>, 4>& m1, const std::array<std::array<double, 4>, 4>& m2) {
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
        return { {
            {temp[0][0], temp[1][0], temp[2][0], temp[3][0]}
        } };
    
}
// m.mul(m1, m2) 4x4 x 1x4 matrix multiplication
std::array<std::array<double, 4>, 4> mul(const std::array<std::array<double, 4>, 4>& m1, const std::array<double, 4>& m2) {
    std::array<std::array<double, 4>, 4> result = mat4x4();

    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            result[i][0] += m1[i][k] * m2[k];
        }
    }

    return result;
}
// m.translate(x, y, z) vector translation
std::array<std::array<double, 4>, 4> translate(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> temp = mat4x4(true);

    temp[0][3] = x;
    temp[1][3] = y;
    temp[2][3] = z;

    return temp;
}
// m.rotationX(angle)
std::array<std::array<double, 4>, 4> rotationX(double angle) {
    double radAngle = angle * pi / 180;
    std::array<std::array<double, 4>, 4> temp = mat4x4(true);
    temp[1][1] = std::cos(radAngle);
    temp[2][1] = -std::sin(radAngle);
    temp[1][2] = std::sin(radAngle);
    temp[2][2] = std::cos(radAngle);
    return temp;
}
// m.rotationY(angle)
std::array<std::array<double, 4>, 4> rotationY(double angle) {
    double radAngle = angle * pi / 180;
    std::array<std::array<double, 4>, 4> temp = mat4x4(true);
    temp[0][0] = std::cos(radAngle);
    temp[2][0] = std::sin(radAngle);
    temp[0][2] = -std::sin(radAngle);
    temp[2][2] = std::cos(radAngle);

    return temp;
}
// m.rotationZ(angle)
std::array<std::array<double, 4>, 4> rotationZ(double angle) {
    double radAngle = angle * pi / 180;
    std::array<std::array<double, 4>, 4> temp = mat4x4(true);
    temp[0][0] = std::cos(radAngle);
    temp[1][0] = -std::sin(radAngle);
    temp[0][1] = std::sin(radAngle);
    temp[1][1] = std::cos(radAngle);

    return temp;
}
// m.rotateXYZ(x, y, z)
std::array<std::array<double, 4>, 4> rotateXYZ(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> rX = rotationX(x);
    std::array<std::array<double, 4>, 4> rY = rotationY(y);
    std::array<std::array<double, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<double, 4>, 4> f = mul(rZ, rY);
    return mul(f, rX);
}
// m.rotateYXZ(x, y, z)
std::array<std::array<double, 4>, 4> rotateYXZ(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> rX = rotationX(x);
    std::array<std::array<double, 4>, 4> rY = rotationY(y);
    std::array<std::array<double, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<double, 4>, 4> f = mul(rZ, rX);
    return mul(f, rY);
}
// m.rotateYZX(x, y, z)
std::array<std::array<double, 4>, 4> rotateYZX(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> rX = rotationX(x);
    std::array<std::array<double, 4>, 4> rY = rotationY(y);
    std::array<std::array<double, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<double, 4>, 4> f = mul(rX, rZ);
    return mul(f, rY);
}
// m.rotateZXY(x, y, z)
std::array<std::array<double, 4>, 4> rotateZXY(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> rX = rotationX(x);
    std::array<std::array<double, 4>, 4> rY = rotationY(y);
    std::array<std::array<double, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<double, 4>, 4> f = mul(rY, rX);
    return mul(f, rZ);
}
// m.rotateZYX(x, y, z)
std::array<std::array<double, 4>, 4> rotateZYX(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> rX = rotationX(x);
    std::array<std::array<double, 4>, 4> rY = rotationY(y);
    std::array<std::array<double, 4>, 4> rZ = rotationZ(z);

    std::array<std::array<double, 4>, 4> f = mul(rX, rY);
    return mul(f, rZ);
}
// m.scale(x, y, z) 
std::array<std::array<double, 4>, 4> scale(double x, double y, double z) {
    std::array<std::array<double, 4>, 4> temp = mat4x4(true);

    temp[0][0] = x;
    temp[1][1] = y;
    temp[2][2] = z;

    return temp;
}
// m.perspective(sizeX, sizeY, nearPlane, far, FOV)
std::array<std::array<double, 4>, 4> perspective(double sizeX, double sizeY, double nearPlane, double farPlane, double FOV) {
    double aspect = sizeX / sizeY;

    double top = nearPlane * std::tan((FOV * pi / 180)/2);
    double bottom = -top;
    double right = top * aspect;
    double left = -right;

    std::array<std::array<double, 4>, 4> temp = mat4x4();

    temp[0] = { (2 * nearPlane) / (right - left), 0.0, (right + left) / (right - left), 0.0 };
    temp[1] = { 0.0, (2 * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0 };
    temp[2] = { 0.0, 0.0, -(farPlane + nearPlane) / (farPlane - nearPlane), -(2 * farPlane * nearPlane) / (farPlane - nearPlane) };
    temp[3] = { 0.0, 0.0, -1.0, 0.0 };

    return temp;
}
// m.divW(vec)
std::array<double, 4> divW(const std::array<double, 4>& vec) {
    std::array<double, 4> temp = vec4();
    temp[0] = vec[0] / vec[3];
    temp[1] = vec[1] / vec[3];
    temp[2] = vec[2] / vec[3];
    return temp;
}
// m.cross4(x1, y1, x2, y2, x3, y3, x4, y4)
int cross4(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}
// m.cross2(v1, v2)
std::array<double, 3> cross2(const std::array<double, 3>& v1, const std::array<double, 3>& v2) {
    double x = v1[1] * v2[2] - v1[2] * v2[1];
    double y = v1[2] * v2[0] - v1[0] * v2[2];
    double z = v1[0] * v2[1] - v1[1] * v2[0];

    return { x, y, z };
}
// m.sub(v1, v2)
std::array<std::array<double, 3>, 3> sub(const std::array<std::array<double, 3>, 3>& v1, const std::array<std::array<double, 3>, 3>& v2) {
    std::array<std::array<double, 3>, 3> result;
    int n = std::min(v1.size(), v2.size()); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < v1[i].size(); ++j) {
            result[i][j] = v1[i][j] - v2[i][j];
        }
    }

    return result;
}
// m.normalize(v)
std::array<double, 3> normalize(const std::array<double, 3>& v) {
    double magnitude = 0.0;
    for (int i = 0; i < v.size(); ++i) {
        magnitude += v[i] * v[i];
    }

    magnitude = std::sqrt(magnitude);

    std::array<double, 3> result;
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] / magnitude;
    }

    return result;
}
// m.dot(v1, v2)
double dot(const std::array<std::array<double, 1>, 3>& v1, const std::array<std::array<double, 1>, 3>& v2) {
    double dotProduct = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        dotProduct += v1[i][0] * v2[i][0];
    }
    return dotProduct;
}
// m.length(v)
double length(const std::array<std::array<double, 1>, 3>& v) {
    double magnitudeSquared = 0.0;
    for (int i = 0; i < v.size(); ++i) {
        magnitudeSquared += v[i][0] * v[i][0];
    }
    return std::sqrt(magnitudeSquared);
}
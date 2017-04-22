#pragma once
#include "Matrix.hpp"
#include "primitives.hpp"
using CTM_t = Matrix<4, 4, double>;
using Vector4_t = std::array<double, 4>;
using Polygon_t = std::vector<Point4D>;
using Line_t = std::array<Point4D, 2>;
using Plane_t = std::array<Point4D, 3>;
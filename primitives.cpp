#include "primitives.hpp"

double getRadianFromDegree(int angle)
{
	static constexpr double Pi = 3.141592653589793238462643383279502884;
	return (angle * Pi / 180.0);
}

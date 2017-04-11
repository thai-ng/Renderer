#include "PointLighter.hpp"
#include "lerp.hpp"

void PointLighter::calculateAmbientLight(std::vector<Point4D>& points, const Color& ambientColor)
{
	auto colorChannels = ambientColor.getNormalizedColorChannels();
	auto ambientRed = std::get<0>(colorChannels);
	auto ambientGreen = std::get<1>(colorChannels);
	auto ambientBlue = std::get<2>(colorChannels);
	
	for (auto& point : points)
	{
		auto pointColorChannels = point.color.getNormalizedColorChannels();
		auto pointRed = std::get<0>(pointColorChannels);
		auto pointGreen = std::get<1>(pointColorChannels);
		auto pointBlue = std::get<2>(pointColorChannels);

		auto newColor = Color::getDenormalizedColor(ambientRed * pointRed, ambientGreen * pointGreen, ambientBlue * pointBlue);
		point.color = newColor;
	}
}

inline
double fatt(double A, double B, double d)
{
	return 1 / (std::fma(B, d, A));
}

inline
double distance(const Point4D& l, const Point4D& p)
{
	return std::sqrt(std::pow(l.x - p.x, 2) + std::pow(l.y - p.y, 2) + std::pow(l.z - p.z, 2));
}

inline
Point4D reflect(const Point4D& l, const Point4D& n)
{
	return l - n * 2 * dot(l, n);
}

Color PointLighter::calculateLightAtPixel(const Point4D& p, const Point4D& vN, const Light& l, double ks, double kp)
{
	auto pl = Point4D{ l.Location[0], l.Location[1], l.Location[2], l.Location[3] };
	auto vl = normalize(pl - p);
	auto vr = reflect(vl, vN);

	auto lightColor = l.color * fatt(l.A, l.B, distance(pl, p)) * (p.color * dot(vN, vl) + ks * std::pow(dot(normalize(p), vr), kp));
	return lightColor;
}

Color PointLighter::calculateLights(Point4D & p, std::vector<Light> & lights, double ks, double kp)
{
	auto vN = normalize(p.normal.value());
	Color c{ 0.0 };
	for (auto& l : lights)
	{
		c = c + calculateLightAtPixel(p, vN, l, ks, kp);
	}
	return c;
}

void PointLighter::calculateLighting(std::vector<Point4D>& points, const Color & ambientColor, std::vector<Light>& lights, double ks, double kp)
{
	Color pointColor{ 255, 255, 255 };
	auto ambientTerm = pointColor * ambientColor;

	for (auto& p : points)
	{
		p.color = ambientTerm + calculateLights(p, lights, ks, kp);
	}
}

void PointLighter::calcuateDepthShading(std::vector<Point4D>& points, const Depth & depth)
{
	auto zLerp = Lerp<double>(depth.near, depth.far, 0.0, 1.0);

	for (auto& point : points)
	{
		if (point.z > depth.far)
		{
			point.color = depth.color;
		}
		else if (point.z > depth.near)
		{
			auto depthColorFactor = zLerp[static_cast<int>(point.z - depth.near)].second;
			auto depthColorChannels = depth.color.getNormalizedColorChannels();
			auto pointColorChannels = point.color.getNormalizedColorChannels();
			auto newRed = ((1 - depthColorFactor) * std::get<0>(pointColorChannels)) + (depthColorFactor * std::get<0>(depthColorChannels));
			auto newGreen = ((1 - depthColorFactor) * std::get<1>(pointColorChannels)) + (depthColorFactor * std::get<1>(depthColorChannels));
			auto newBlue = ((1 - depthColorFactor) * std::get<2>(pointColorChannels)) + (depthColorFactor * std::get<2>(depthColorChannels));

			point.color = Color::getDenormalizedColor(newRed, newGreen, newBlue);
		}
	}
}

//for (auto& point : points)
//{
//	if (point.z > depth.far)
//	{
//		point.color = depth.color;
//	}
//	else if (point.z > depth.near)
//	{
//		auto factor = (point.z - depth.near) / (depth.far - depth.near);
//		auto newColor = point.color * factor + (1 - factor) * depth.color;
//		point.color = newColor;
//	}
//}
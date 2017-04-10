#include "PointLighter.hpp"
#include "lerp.hpp"

void PointLighter::calculateAmbientLight(std::vector<Point>& points, const Color& ambientColor)
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
//
//void PointLighter::calculateLighting(std::vector<Point>& points, const Color & ambientColor, std::vector<Light>& lights, double A, double B)
//{
//	Color pointColor{ 255, 255, 255 };
//	auto ambientTerm = pointColor * ambientColor;
//
//
//}

void PointLighter::calcuateDepthShading(std::vector<Point>& points, const Depth & depth)
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
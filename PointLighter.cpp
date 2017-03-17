#include "PointLighter.hpp"

void PointLighter::calculateLight(std::vector<Point>& points, const Color& ambientColor)
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

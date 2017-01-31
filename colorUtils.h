#pragma once
#include <tuple>

typedef std::tuple<unsigned int, unsigned int, unsigned int> ColorChannels;

ColorChannels getColorChannels(unsigned int color)
{
	auto blue = color << 24 >> 24;
	auto green = color << 16 >> 24;
	auto red = color << 8 >> 24;
	return std::make_tuple(red, green, blue);
}

unsigned int getColorFromChannels(unsigned int r, unsigned int g, unsigned int b)
{
	return 0xff000000 | (r << 16) | (g << 8) | b;
}

unsigned int getColorWithOpacity(unsigned int oldColor, unsigned int newColor, double opacity)
{
	auto oldColorChannels = getColorChannels(oldColor);
	auto oldRed = std::get<0>(oldColorChannels);
	auto oldGreen = std::get<1>(oldColorChannels);
	auto oldBlue = std::get<2>(oldColorChannels);

	auto newColorChannels = getColorChannels(newColor);
	auto newRed = std::get<0>(newColorChannels);
	auto newGreen = std::get<1>(newColorChannels);
	auto newBlue = std::get<2>(newColorChannels);

	auto red = static_cast<unsigned int>((opacity * newRed) + ((1 - opacity)*oldRed));
	auto green = static_cast<unsigned int>((opacity * newGreen) + ((1 - opacity)*oldGreen));
	auto blue = static_cast<unsigned int>((opacity * newBlue) + ((1 - opacity)*oldBlue));
	return getColorFromChannels(red, green, blue);
}

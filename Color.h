#pragma once
#include <tuple>

typedef std::tuple<unsigned char, unsigned char, unsigned char> ColorChannels;

class Color
{
public:
	Color(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
	
	Color(double color)
	{
		r = static_cast<unsigned char>(0x00ffffff << 8 >> 24);
		r = static_cast<unsigned char>(r * color);

		g = static_cast<unsigned char>(0x00ffffff << 16 >> 24);
		g = static_cast<unsigned char>(g * color);

		b = static_cast<unsigned char>(0x00ffffff << 24 >> 24);
		b = static_cast<unsigned char>(b * color);
	}

	Color(unsigned int color)
	{
		r = static_cast<unsigned char>(color << 8 >> 24);
		g = static_cast<unsigned char>(color << 16 >> 24);
		b = static_cast<unsigned char>(color << 24 >> 24);
	}

	unsigned int asUnsigned() const
	{
		return 0xff000000 | (r << 16) | (g << 8) | b;
	}

	ColorChannels getColorChannels() const
	{
		return std::make_tuple(r, g, b);
	}

	bool operator==(const Color& other) const
	{
		return other.asUnsigned() == asUnsigned();
	}
private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

Color colorWithOpacity(const Color& newColor, const Color& oldColor, double opacity)
{
	auto oldColorChannels = oldColor.getColorChannels();
	auto oldRed = std::get<0>(oldColorChannels);
	auto oldGreen = std::get<1>(oldColorChannels);
	auto oldBlue = std::get<2>(oldColorChannels);

	auto newColorChannels = newColor.getColorChannels();
	auto newRed = std::get<0>(newColorChannels);
	auto newGreen = std::get<1>(newColorChannels);
	auto newBlue = std::get<2>(newColorChannels);

	auto red = static_cast<unsigned char>((opacity * newRed) + ((1 - opacity)*oldRed));
	auto green = static_cast<unsigned char>((opacity * newGreen) + ((1 - opacity)*oldGreen));
	auto blue = static_cast<unsigned char>((opacity * newBlue) + ((1 - opacity)*oldBlue));
	return Color(red, green, blue);
}
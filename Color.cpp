#include "Color.hpp"

unsigned char denormalizeColor(double color)
{
	unsigned char result;
	if (color > 1.0)
	{
		result = 255;
	}
	else if (color < 0)
	{
		result = 0;
	}
	else
	{
		result = static_cast<unsigned char>(color * 255.0);
	}

	return result;
}

Color Color::getDenormalizedColor(double r, double g, double b)
{
	Color result{ 0, 0, 0 };
	result.r = static_cast<unsigned char>(denormalizeColor(r));
	result.g = static_cast<unsigned char>(denormalizeColor(g));
	result.b = static_cast<unsigned char>(denormalizeColor(b));
	return result;
}


Color::Color(double color)
{
	r = static_cast<unsigned char>(0x00ffffff << 8 >> 24);
	r = static_cast<unsigned char>(r * color);

	g = static_cast<unsigned char>(0x00ffffff << 16 >> 24);
	g = static_cast<unsigned char>(g * color);

	b = static_cast<unsigned char>(0x00ffffff << 24 >> 24);
	b = static_cast<unsigned char>(b * color);
}

Color::Color(unsigned int color)
{
	r = static_cast<unsigned char>(color << 8 >> 24);
	g = static_cast<unsigned char>(color << 16 >> 24);
	b = static_cast<unsigned char>(color << 24 >> 24);
}

unsigned int Color::asUnsigned() const
{
	return 0xff000000 | (r << 16) | (g << 8) | b;
}

ColorChannels Color::getColorChannels() const
{
	return std::make_tuple(r, g, b);
}

NormalizedColorChannels Color::getNormalizedColorChannels() const
{
	auto red = static_cast<double>(r) / 255.0;
	auto green = static_cast<double>(g) / 255.0;
	auto blue = static_cast<double>(b) / 255.0;

	return std::make_tuple(red, green, blue);
}

bool Color::operator==(const Color& other) const
{
	return other.asUnsigned() == asUnsigned();
}

Color operator*(const Color& l, const Color& r)
{
	auto leftChannels = l.getNormalizedColorChannels();
	auto lr = std::get<0>(leftChannels);
	auto lg = std::get<1>(leftChannels);
	auto lb = std::get<2>(leftChannels);

	auto rightChannels = r.getNormalizedColorChannels();
	auto rr = std::get<0>(rightChannels);
	auto rg = std::get<1>(rightChannels);
	auto rb = std::get<2>(rightChannels);

	return Color::getDenormalizedColor(lr*rr, lg*rg, lb*rb);
}

Color operator+(const Color & l, const Color & r)
{
	auto leftChannels = l.getNormalizedColorChannels();
	auto lr = std::get<0>(leftChannels);
	auto lg = std::get<1>(leftChannels);
	auto lb = std::get<2>(leftChannels);

	auto rightChannels = r.getNormalizedColorChannels();
	auto rr = std::get<0>(rightChannels);
	auto rg = std::get<1>(rightChannels);
	auto rb = std::get<2>(rightChannels);

	return Color::getDenormalizedColor(lr+rr, lg+rg, lb+rb);
}

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
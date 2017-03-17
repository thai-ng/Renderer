#include "command.hpp"

using namespace std::string_literals;

static const std::unordered_map<std::string, Command::Operation> OperationTokens{
	{ "{"s, Command::Operation::OpenBrace },
	{ "}"s, Command::Operation::CloseBrace },
	{ "scale"s, Command::Operation::Scale },
	{ "rotate"s, Command::Operation::Rotate },
	{ "translate"s, Command::Operation::Translate },
	{ "line"s, Command::Operation::Line },
	{ "polygon"s, Command::Operation::Polygon },
	{ "file"s, Command::Operation::File },
	{ "wire"s, Command::Operation::Wire },
	{ "filled"s, Command::Operation::Filled },
	{ "camera"s, Command::Operation::Camera},
	{ "obj"s, Command::Operation::ObjectFile},
	{ "ambient"s, Command::Operation::Ambient},
	{ "depth"s, Command::Operation::Depth},
	{ "surface"s, Command::Operation::Surface},
	{ "v"s, Command::Operation::Vertex},
	{ "vn"s, Command::Operation::VertexNormal},
	{ "f"s, Command::Operation::Face}
};

static const std::unordered_map<std::string, Axis> AxisTokens{
	{ "X"s, Axis::X },
	{ "Y"s, Axis::Y },
	{ "Z"s, Axis::Z }
};

Command::Command(const std::vector<std::string>& tokens)
{
	_op = OperationTokens.at(tokens[0]);
	switch (_op)
	{
		case Command::Operation::Polygon:
		{
			if (tokens.size() == 10)
			{
				params = PolygonParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, Color{255, 255, 255} },
										Point4D{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), 1, Color{255, 255, 255} },
										Point4D{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()), 1, Color{255, 255, 255} } };
			}
			else
			{
				auto color1 = Color::getDenormalizedColor(std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()));
				auto color2 = Color::getDenormalizedColor(std::atof(tokens[10].c_str()), std::atof(tokens[11].c_str()), std::atof(tokens[12].c_str()));
				auto color3 = Color::getDenormalizedColor(std::atof(tokens[16].c_str()), std::atof(tokens[17].c_str()), std::atof(tokens[18].c_str()));
				params = PolygonParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, color1 },
										Point4D{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), 1, color2 },
										Point4D{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()), 1, color3 } };
			}
		} break;

		case Command::Operation::Line:
		{
			// no color
			if (tokens.size() == 7)
			{
				params = LineParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, Color{ 255, 255, 255 } },
									 Point4D{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), 1, Color{ 255, 255, 255 } } };
			}
			else
			{
				auto color1 = Color::getDenormalizedColor(std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()));
				auto color2 = Color::getDenormalizedColor(std::atof(tokens[10].c_str()), std::atof(tokens[11].c_str()), std::atof(tokens[12].c_str()));
				params = LineParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, color1 },
				 					 Point4D{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()), 1, color2 } };

			}
		} break;

		case Command::Operation::Translate:
		case Command::Operation::Scale:
		{
			params = Vector3{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()) };
		} break;

		case Command::Operation::Rotate:
		{
			params = RotateParams{ AxisTokens.at(tokens[1]), std::atoi(tokens[2].c_str()) };
		} break;

		case Command::Operation::File:
		{
			auto fileName = std::string(std::next(tokens[1].cbegin()), std::prev(tokens[1].cend()));
			fileName += ".simp";
			params = fileName;
		} break;

		case Command::Operation::Ambient:
		{
			params = Color::getDenormalizedColor( std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()));
		} break;

		case Command::Operation::Camera:
		{
			params = CameraParams{ std::atof(tokens[1].c_str()),	// xLow
								   std::atof(tokens[2].c_str()),	// xHigh
								   std::atof(tokens[3].c_str()),	// yLow
								   std::atof(tokens[4].c_str()),	// yHigh
								   std::atof(tokens[5].c_str()),	// near
								   std::atof(tokens[6].c_str()) };	// far
		} break;

		default:
		{

		} break;
	}
}

Command::Operation Command::operation() const
{
	return _op;
}

const CommandParams& Command::parameters() const
{
	return params;
}
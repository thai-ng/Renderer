#include "command.hpp"

using namespace std::string_literals;

static const std::unordered_map<std::string, Axis> AxisTokens{
	{ "X"s, Axis::X },
	{ "Y"s, Axis::Y },
	{ "Z"s, Axis::Z }
};

Command::Command(const std::vector<std::string>& tokens)
{
	static Color defaultVertexColor = Color{ 255, 255, 255 };

	_op = OperationTokens.at(tokens[0]);
	switch (_op)
	{
		case Command::Operation::Polygon:
		{
			if (tokens.size() == 10)
			{
				params = PolygonParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, defaultVertexColor },
										Point4D{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), 1, defaultVertexColor },
										Point4D{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()), 1, defaultVertexColor } };
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
				params = LineParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, defaultVertexColor },
									 Point4D{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), 1, defaultVertexColor } };
			}
			else
			{
				auto color1 = Color::getDenormalizedColor(std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()));
				auto color2 = Color::getDenormalizedColor(std::atof(tokens[10].c_str()), std::atof(tokens[11].c_str()), std::atof(tokens[12].c_str()));
				params = LineParams{ Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, color1 },
				 					 Point4D{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()), 1, color2 } };

			}
		} break;

		case Command::Operation::VertexNormal:
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

		case Command::Operation::ObjectFile:
		{
			auto fileName = std::string(std::next(tokens[1].cbegin()), std::prev(tokens[1].cend()));
			fileName += ".obj";
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

		case Command::Operation::Depth:
		{
			params = DepthParams{ std::atof(tokens[1].c_str()),
								  std::atof(tokens[2].c_str()),
								  Color::getDenormalizedColor(std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str())) };
		} break;

		case Command::Operation::Surface:
		{
			defaultVertexColor = Color::getDenormalizedColor(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()));
		} break;

		case Command::Operation::Vertex:
		{
			// v x y z w r g b
			if (tokens.size() == 8)
			{
				auto color = Color::getDenormalizedColor(std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()), std::atof(tokens[7].c_str()));
				params = Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str()), color };
			}
			// v x y z r g b
			else if (tokens.size() == 7)
			{
				auto color = Color::getDenormalizedColor(std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()));
				params = Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, color };
			}
			// v x y z w
			else if (tokens.size() == 5)
			{
				params = Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str()), defaultVertexColor };
			}
			// v x y z
			else
			{
				params = Point4D{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()), 1, defaultVertexColor };
			}
		} break;

		case Command::Operation::Face:
		{
			FaceParam faceParams;
			faceParams.resize(tokens.size() - 1);
			std::transform(std::next(tokens.begin()), 
						   tokens.end(), 
						   faceParams.begin(), 
						   [](auto& token) 
						   {
								// normal
								auto normalDelimIndex = token.find("//");
								if (normalDelimIndex != std::string::npos)
								{
									auto vertex = std::atoi(std::string(token.begin(), token.begin() + normalDelimIndex + 1).c_str());
									auto normal = std::atoi(std::string(token.begin() + normalDelimIndex + 3, token.end()).c_str());

									return Vertex{ vertex, 0, normal };
								}
								else
								{
									auto textureDelimIndex = token.find("/");
									// texture
									if (textureDelimIndex != std::string::npos)
									{
										auto vertex = std::atoi(std::string(token.begin(), token.begin() + textureDelimIndex + 1).c_str());
										normalDelimIndex = token.find("/", textureDelimIndex + 1);
										// texture + normal
										if (normalDelimIndex != std::string::npos)
										{
											auto texture = std::atoi(std::string(token.begin() + textureDelimIndex + 2, token.begin() + normalDelimIndex + 1).c_str());
											auto normal = std::atoi(std::string(token.begin() + normalDelimIndex + 2, token.end()).c_str());
											return Vertex{ vertex, texture, normal };
										}
										else
										{
											auto texture = std::atoi(std::string(token.begin() + textureDelimIndex + 2, token.end()).c_str());
											return Vertex{ vertex, texture, 0 };
										}
									}
									// vertex only
									else
									{
										auto vertex = std::atoi(token.c_str());
										return Vertex{ vertex, 0, 0 };
									}
								}
						   });

			params = faceParams;
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
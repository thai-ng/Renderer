#pragma once
#include <unordered_map>
#include <string>
#include <utility>
#include <array>
#include <variant>
#include <cstdlib>

enum class Operation
{
	OpenBrace,
	CloseBrace,
	Scale,
	Rotate,
	Translate,
	Line,
	Polygon,
	File,
	Wire,
	Filled
};

enum class Axis
{
	X,
	Y,
	Z
};

using namespace std::string_literals;

static const std::unordered_map<std::string, Operation> OperationTokens{
	{"{"s, Operation::OpenBrace},
	{"}"s, Operation::CloseBrace},
	{"scale"s, Operation::Scale},
	{"rotate"s, Operation::Rotate},
	{"translate"s, Operation::Translate},
	{"line"s, Operation::Line},
	{"polygon"s, Operation::Polygon},
	{"file"s, Operation::File},
	{"wire"s, Operation::Wire},
	{"filled"s, Operation::Filled}
};

static const std::unordered_map<std::string, Axis> AxisTokens {
	{"X"s, Axis::X},
	{"Y"s, Axis::Y},
	{"Z"s, Axis::Z}
};

class Command
{
public:
	Command(const std::vector<std::string>& tokens)
	{
		op = OperationTokens.at(tokens[0]);
		switch (op)
		{
			case Operation::Polygon:
			{
				params = PolygonParams{ Vector3{std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str())},
										Vector3{std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str())}, 
										Vector3{std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str())} };
			} break;

			case Operation::Line:
			{
				params = LineParams{ Vector3{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()) },
									 Vector3{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()) } };
			} break;

			case Operation::Translate:
			case Operation::Scale:
			{
				params = Vector3{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()) };
			} break;

			case Operation::Rotate:
			{
				params = RotateParams{ AxisTokens.at(tokens[1]), std::atoi(tokens[2].c_str()) };
			} break;
			
			case Operation::File:
			{
				params = std::string(std::next(tokens[1].cbegin()), std::prev(std::prev(tokens[1].cend())));
			} break;

			default:
			{

			} break;
		}
	}

private:
	typedef std::array<double, 3> Vector3;
	typedef std::array<Vector3, 3> PolygonParams;
	typedef std::array<Vector3, 2> LineParams;
	typedef std::pair<Axis, int> RotateParams;
	typedef std::string FileParam;

	Operation op;
	std::variant<Vector3, PolygonParams, LineParams, RotateParams, FileParam> params;
	
};
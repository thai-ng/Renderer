#pragma once
#include <unordered_map>
#include <string>
#include <utility>
#include <array>
#include <variant>
#include <cstdlib>

#include "primitives.hpp"

enum class Axis
{
	X,
	Y,
	Z
};

struct CameraParams
{
	double xLow;
	double yLow;
	double xHigh;
	double yHigh;
	double near;
	double far;
};

struct DepthParams
{
	double near;
	double far;
	Color color;
};

using Vector3 = std::array<double, 3>;
using PolygonParams = std::array<Point4D, 3>;
using LineParams = std::array<Point4D, 2>;
using RotateParams = std::pair<Axis, int>;
using FileParam = std::string;
using Vertex = std::array<int, 3>;
using FaceParam = std::vector<Vertex>;
using LightParams = std::array<double, 5>;
using CommandParams = std::variant<Vector3, PolygonParams, LineParams, RotateParams, FileParam, Color, CameraParams, DepthParams, Point4D, FaceParam, LightParams, LightingMethod>;

class Command
{
public:
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
		Filled,
		Camera,
		ObjectFile,
		Ambient,
		Depth,
		Surface,
		Vertex,
		VertexNormal,
		Face,
		Light,
		Phong,
		Gouraud,
		Flat
	};


	Command(Operation op, CommandParams&& parameters) : _op(op), params(parameters) {}

	Command(const std::vector<std::string>& tokens);

	Operation operation() const;

	const CommandParams& parameters() const;

private:
	Operation _op;
	CommandParams params;
};

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
	{ "camera"s, Command::Operation::Camera },
	{ "obj"s, Command::Operation::ObjectFile },
	{ "ambient"s, Command::Operation::Ambient },
	{ "depth"s, Command::Operation::Depth },
	{ "surface"s, Command::Operation::Surface },
	{ "v"s, Command::Operation::Vertex },
	{ "vn"s, Command::Operation::VertexNormal },
	{ "f"s, Command::Operation::Face },
	{ "light"s, Command::Operation::Light },
	{ "phong"s, Command::Operation::Phong },
	{ "gouraud"s, Command::Operation::Gouraud },
	{ "flat"s, Command::Operation::Flat }
};
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
	double xHigh;
	double yLow;
	double yHigh;
	double near;
	double far;
};

using Vector3 = std::array<double, 3>;
using PolygonParams = std::array<Point4D, 3>;
using LineParams = std::array<Point4D, 2>;
using RotateParams = std::pair<Axis, int>;
using FileParam = std::string;
using CommandParams = std::variant<Vector3, PolygonParams, LineParams, RotateParams, FileParam, Color, CameraParams>;

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
		Face
	};


	Command(Operation op, CommandParams&& parameters) : _op(op), params(parameters) {}

	Command(const std::vector<std::string>& tokens);

	Operation operation() const;

	const CommandParams& parameters() const;

private:
	Operation _op;
	CommandParams params;
};
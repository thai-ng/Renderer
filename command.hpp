#pragma once
#include <unordered_map>
#include <string>
#include <utility>
#include <array>
#include <variant>
#include <cstdlib>

enum class Axis
{
	X,
	Y,
	Z
};

using Vector3 = std::array<double, 3>;
using PolygonParams = std::array<Vector3, 3>;
using LineParams = std::array<Vector3, 2>;
using RotateParams = std::pair<Axis, int>;
using FileParam = std::string;
using CommandParams = std::variant<Vector3, PolygonParams, LineParams, RotateParams, FileParam>;

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
		Filled
	};


	Command(Operation op, CommandParams&& parameters) : _op(op), params(parameters) {}

	Command(const std::vector<std::string>& tokens);

	Operation operation() const;

	const CommandParams& parameters() const;

private:
	Operation _op;
	CommandParams params;
};
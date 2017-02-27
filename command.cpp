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
	{ "filled"s, Command::Operation::Filled }
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
			params = PolygonParams{ Vector3{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()) },
				Vector3{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()) },
				Vector3{ std::atof(tokens[7].c_str()), std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str()) } };
		} break;

		case Command::Operation::Line:
		{
			params = LineParams{ Vector3{ std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()) },
				Vector3{ std::atof(tokens[4].c_str()), std::atof(tokens[5].c_str()), std::atof(tokens[6].c_str()) } };
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
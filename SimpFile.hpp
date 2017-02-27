#pragma once
#include <fstream>
#include "command.hpp"

class SimpFile
{
public:
	SimpFile(const std::string& fileName);

	std::vector<Command> commands() const;

private:
	std::vector<std::string> getTokens(const std::string& line);
	void parseAndAddLine(const std::string& line);

	std::ifstream currentStream;
	std::vector<Command> _commands;
};
#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>

#include "command.h"

class SimpFile
{
public:
	SimpFile(const std::string& fileName) : currentStream(fileName)
	{
		if (currentStream)
		{
			std::string line;
			while (std::getline(currentStream, line))
			{
				parseAndAddLine(line);
			}
		}
		else
		{
			throw std::invalid_argument("Cannot open file");
		}
	}

private:
	std::ifstream currentStream;
	std::vector<Command> commands;

	std::vector<std::string> getTokens(const std::string& line)
	{
		std::vector<std::string> result;
		static auto findDelimLambda = [](const char c)
		{
			return (c == ' ' || c == '\t' || c == ',' || c == '\n ' || c == '(' || c == ')');
		};
		
		auto currentIter = line.cbegin();

		auto nextDelim = currentIter;
		while ((nextDelim = std::find_if(currentIter, line.cend(), findDelimLambda)) != line.cend())
		{
			if (std::distance(currentIter, nextDelim) > 0)
			{
				auto token = std::string(currentIter, nextDelim);
				result.push_back(token);
			}
			// If same as current
			else
			{
				// Move forward once
				nextDelim = std::next(nextDelim);
			}
			// Move the search forward
			currentIter = nextDelim;
		}

		if (std::distance(currentIter, nextDelim) > 0)
		{
			auto token = std::string(currentIter, nextDelim);
			result.push_back(token);
		}

		return result;
	}

	void parseAndAddLine(const std::string& line)
	{
		auto tokens = getTokens(line);
		if (tokens.size() > 0 && tokens[0] != "#")
		{
			// Not empty line or comment
			Command command(tokens);
			commands.push_back(command);

		}
	}
};
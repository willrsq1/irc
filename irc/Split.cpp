
#include "Commands.hpp"

std::vector<std::string> split(std::string & str, char delim)
{
	std::vector<std::string> tokens;

	std::string token;

	std::istringstream tokenStream(str.c_str());

	while (std::getline(tokenStream, token, delim))
	{
		tokens.push_back(token);
	}
	return (tokens);
}
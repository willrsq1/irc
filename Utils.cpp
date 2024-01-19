
#include "Commands.hpp"

std::vector<std::string> split(std::string & str, char delim)
{
	std::vector<std::string> tokens;

	std::string token;

	std::istringstream tokenStream(str.c_str());

	while (std::getline(tokenStream, token, delim))
	{
		if (token[0] == ':')
		{
			token.erase(0, 1);
			tokenStream >> token;
			std::cout << "token is: " << token << std::endl;
		}
		tokens.push_back(token);
	}
	return (tokens);
}

bool	isAlreadyUsed(std::string const & str, std::string const & str2)
{
	std::string strToUpper = str;
	std::transform(strToUpper.begin(), strToUpper.end(), strToUpper.begin(), ::toupper);

	std::string str2ToUpper = str2;
	std::transform(str2ToUpper.begin(), str2ToUpper.end(), str2ToUpper.begin(), ::toupper);

	if (strToUpper == str2ToUpper)
		return (true);
	return (false);
}
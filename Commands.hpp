#ifndef INPUTS_HPP
# define INPUTS_HPP	

#include <iostream>
#include <sstream>
#include <algorithm>

#include "Server.hpp"
#include "Client.hpp"

void	join(Server & server, Client & client, std::vector<std::string> & commands);
void	pass(Server & server, Client & client, std::vector<std::string> & commands);
void	nick(Server & server, Client & client, std::vector<std::string> & commands);
void	user(Server & server, Client & client, std::vector<std::string> & commands);
void	privmsg(Server & server, Client & client, std::vector<std::string> & commands);
void	part(Server & server, Client & client, std::vector<std::string> & commands);
void	kick(Server & server, Client & client, std::vector<std::string> & commands);







std::vector<std::string> split(std::string & str, char delim);
std::vector<std::string> basicSplit(std::string & str, char delim);
bool	caseInsensitiveCheck(std::string const & str, std::string const & str2);


#endif
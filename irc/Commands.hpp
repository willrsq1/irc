#ifndef INPUTS_HPP
# define INPUTS_HPP	

#include <iostream>
#include <sstream>

#include "Server.hpp"
#include "Client.hpp"

void	join(Server & server, Client & client, std::vector<std::string> & commands);
void	pass(Server & server, Client & client, std::vector<std::string> & commands);
void	nick(Server & server, Client & client, std::vector<std::string> & commands);








std::vector<std::string> split(std::string & str, char delim);


#endif
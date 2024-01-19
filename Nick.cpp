#include "Commands.hpp"

void	nick(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (commands.size() < 2 || commands[1].empty()) // no parameters
	{
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return ;
	}

	if (commands.size() > 2) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), commands[1], "Too many parameters"));
		return ;
	}

	if (commands[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\{|}") != std::string::npos) // invalid characters from RFC
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), commands[1], "Invalid characters in nickname"));
		return ;
	}

	for (it_clients it = server.getClientsBegin() ; it != server.getClientsEnd() ; it++) //checks if nickname already used
	{

		if (caseInsensitiveCheck(commands[1], it->second->getNickname()) == true) //checks with CASE INSENSITIVE
		{
			server.sendToClient(client.getFd(), ERR_NICKNAMEINUSE(client.getNickname(), commands[1]));
			return ;
		}
		
	}

	for (it_clients it = server.getClientsBegin() ; it != server.getClientsEnd() ; it++) //sends to everyone the new nickname
	{

		if (it->second->getFd() != client.getFd())
		{
			server.sendToClient(it->second->getFd(), RPL_NEWNICKNAME(client.getNickname(), client.getUsername(), commands[1]));
		}

	}

	client.setNickname(commands[1]);
	client.setHasSetNickname(true); //client has set his nickname, he can now use USER command

}
#include "Commands.hpp"

void	nick(Server & server, Client & client, std::vector<std::string> & commands)
{

	//check if too mani parameters

	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), commands[1], "Too many parameters"));
		return ;
	}

	//check if no parameters

	if (commands.size() < 2 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return ;
	}

	//check if invalid characters

	if (commands[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\{|}") != std::string::npos)
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getNickname(), commands[1], "Invalid characters in nickname"));
		return ;
	}

	//check if nickname already in use

	for (it_clients it = server.getClientsBegin() ; it != server.getClientsEnd() ; it++)
	{
		if (isAlreadyUsed(commands[1], it->second->getNickname()) == true)
		{
			server.sendToClient(client.getFd(), ERR_NICKNAMEINUSE(client.getNickname(), commands[1]));
			return ;
		}
	}
	
	//send to all clients the update of nickname

	for (it_clients it = server.getClientsBegin() ; it != server.getClientsEnd() ; it++)
	{
		if (it->second->getFd() != client.getFd())
		{
			server.sendToClient(it->second->getFd(), RPL_NEWNICKNAME(client.getNickname(), commands[1]));
		}
	}

//MAYBE TO DO: DIFFEREND MESSAGE FORM

	client.setNickname(commands[1]);
	client.setHasSetNickname(true);
}
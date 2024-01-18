#include "Commands.hpp"

void	nick(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (commands.size() > 2)
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getName(), commands[1], "Too many parameters"));
		return ;
	}

	if (commands.size() < 2 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getName()));
		return ;
	}

	if (commands[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\{|}") != std::string::npos)
	{
		server.sendToClient(client.getFd(), ERR_ERRONEUSNICKNAME(client.getName(), commands[1], "Invalid characters in nickname"));
		return ;
	}

	for (it_clients it = server.getClientsMapBegin() ; it != server.getClientsMapEnd() ; it++)
	{
		if (it->second->getName() == commands[1])
		{
			for (it_clients it = server.getClientsMapBegin() ; it != server.getClientsMapEnd() ; it++)
			{
				server.sendToClient(it->second->getFd(), ERR_NICKNAMEINUSE(client.getName(), commands[1]));
			}
			return ;
		}
	}
	
	for (it_clients it = server.getClientsMapBegin() ; it != server.getClientsMapEnd() ; it++)
	{
		if (it->second->getFd() != client.getFd())
		{
			server.sendToClient(it->second->getFd(), RPL_NEWNICKNAME(client.getName(), commands[1]));
		}
	}

	client.setName(commands[1]);
}
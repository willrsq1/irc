#include "Commands.hpp"

static void	sendmsg(Server & server, Client & client, std::vector<std::string> & commands, std::string const & target);

void	privmsg(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 3)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
		return ;
	}

	if (commands.size() > 3)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT(client.getNickname(), "(PRIVMSG)"));
		return ;
	}

	if (commands[2].empty())
	{
		server.sendToClient(client.getFd(), ERR_NOTEXTTOSEND(client.getNickname()));
		return ;
	}

	std::vector<std::string> tokens = splitPrivMsg(commands[1], ',');
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		sendmsg(server, client, commands, *it);
	}

}

static void	sendmsg(Server & server, Client & client, std::vector<std::string> & commands, std::string const & target)
{
	if (target.empty())
	{
		server.sendToClient(client.getFd(), ERR_NORECIPIENT(client.getNickname(), "(PRIVMSG)"));
		return ;
	}
	if (target[0] == '$' && target.substr(1) == (std::string)SERVER)
	{
		server.sendToAllClients(PRIVMSG(client.getNickname(), client.getUsername(), target, commands[2]));
		return ;
	}
	else if (target[0] != '#')
	{	
		for (it_clients it = server.getClientsMapBegin(); it != server.getClientsMapEnd(); it++)
		{
			if (it->second->getNickname() == target)
			{
				server.sendToClient(it->second->getFd(), PRIVMSG(client.getNickname(), client.getUsername(), target, commands[2]));
				return ;
			}
		}
	}
	server.sendToClient(client.getFd(), ERR_NOSUCHNICK(client.getNickname(), target));
	
}

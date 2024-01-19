#include "Commands.hpp"

void	clientPartingFromChannel(Server & server, Client & client, std::vector<std::string> & commands);

void	part(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (commands.size() < 2 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
		return ;
	}

	if (commands.size() > 3)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	std::vector<std::string> tokens = basicSplit(commands[1], ',');

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		commands[1] = *it;
		clientPartingFromChannel(server, client, commands);
	}


}

void	clientPartingFromChannel(Server & server, Client & client, std::vector<std::string> & commands)
{
	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++)
	{
		if (it->first == commands[1])
		{
			if (it->second->isClientInChannel(client.getFd()))
			{
				server.sendToAllClientsInChannel(it->first, PART(client.getNickname(), client.getRealname(), commands[1], ((commands.size() == 3) ? commands[2] : " ")));
				server.removeClientFromChannel(it->first, client.getFd());
				return ;
			}
			else
			{
				server.sendToClient(client.getFd(), ERR_NOTONCHANNEL(client.getNickname(), commands[1]));
				return ;
			}
		}
	} 

	server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
}
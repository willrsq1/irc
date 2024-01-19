#include "Commands.hpp"

void	newClientInChannel(Server & server, Client & client, Channel & channel);

void	join(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (commands.size() < 2 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return ;
	}

	if (commands.size() > 3)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1][0] != '#')
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), commands[1]));
		return ;
	}

	if (commands[1].size() == 1)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid channel name. Cannot create channel."));
	}

	if (commands[1].find(' ') != std::string::npos || commands[1].find(0x07) != std::string::npos || commands[1].find(',') != std::string::npos)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid characters in channel name. Cannot create channel."));
		return ;
	}

	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++)
	{
		if (it->first == commands[1])
		{
			if (it->second->isClientInChannel(client.getFd()))
			{
				server.sendToClient(client.getFd(), ERR_FATALERROR("You are already in this channel"));
				return ;
			}
			else
			{
				newClientInChannel(server, client, *it->second);
				return ;
			}
		}
	}

	Channel * channel = new Channel(commands[1], &server);
	server.addChannel(commands[1], channel);
	newClientInChannel(server, client, *channel);

}

void	newClientInChannel(Server & server, Client & client, Channel & channel)
{
	channel.addClient(client.getFd(), &client);
	client.addChannel(channel.getName(), &channel);
	(void)server;
	// server.sendToClient(client.getFd(), "coucou bienvenu dans le channel " + channel.getName() + "\r\n");
	// server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), commands[1], it->second->getTopic()));
	// server.sendToClient(client.getFd(), RPL_NAMREPLY(client.getNickname(), commands[1], it->second->getUsers()));
	// server.sendToClient(client.getFd(), RPL_ENDOFNAMES(client.getNickname(), commands[1]));
	// server.sendToClient(client.getFd(), RPL_MOTDSTART(client.getNickname()));
	// server.sendToClient(client.getFd(), RPL_MOTD(client.getNickname()));
	// server.sendToClient(client.getFd(), RPL_ENDOFMOTD(client.getNickname()));
}
#include "Commands.hpp"

void	joinChannel(Server & server, Client & client, std::string & token);
void	newClientInChannel(Server & server, Client & client, Channel & channel);

void	join(Server & server, Client & client, std::vector<std::string> & commands)
{
	
	if (commands.size() < 2 || commands[1].empty()) // no parameters
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return ;
	}

	if (commands.size() > 3) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].size() == 1) // its just "#"
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid channel name. Cannot create channel."));
	}
	//look if channel already exists

	
	std::vector<std::string> tokens = basicSplit(commands[1], ','); // different targets

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{

		joinChannel(server, client, *it); // send message to each target

	}

}

void	joinChannel(Server & server, Client & client, std::string & token)
{
	
	if (token[0] != '#') // its not a channel	
	{
		server.sendToClient(client.getFd(), ERR_NOSUCHCHANNEL(client.getNickname(), token));
		return ;
	}

	if (token.find(' ') != std::string::npos || token.find(0x07) != std::string::npos || token.find(',') != std::string::npos) // invalid characters from RFC
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Invalid characters in channel name. Cannot create channel."));
		return ;
	}


	for (it_channels it = server.getChannelsBegin(); it != server.getChannelsEnd(); it++)
	{

		if (caseInsensitiveCheck(it->first, token)) //does exist (CASE INSENSITIVE)
		{
			if (it->second->isClientInChannel(client.getFd())) //if client already in channel
			{
				server.sendToClient(client.getFd(), ERR_FATALERROR("You are already in this channel"));
				return ;
			}
			if (it->second->isBanned(client.getFd())) //if client is banned
			{
				server.sendToClient(client.getFd(), ERR_FATALERROR("You are banned from this channel"));
				return ;
			}
			newClientInChannel(server, client, *it->second); //if not, add client to channel
			return ;
		}

	}

	//if channel does not exist, create it and add the client to it

	Channel * channel = new Channel(token, &server);
	server.addChannel(token, channel); // register the new channel in the server !
	newClientInChannel(server, client, *channel);

}

void	newClientInChannel(Server & server, Client & client, Channel & channel)
{
	//add the client to the channel, and the channel to the client

	if (channel.limitIsReached() == true)
	{
		server.sendToClient(client.getFd(), ERR_CHANNELISFULL(client.getNickname(), channel.getName()));
		return ;
	}

	channel.addClient(client.getFd(), &client);
	client.addChannel(channel.getName(), &channel);
	server.sendToAllClientsInChannel(channel.getName(), JOIN(client.getNickname(), client.getUsername(), channel.getName()));
	(void)server;
	// server.sendToClient(client.getFd(), "coucou bienvenu dans le channel " + channel.getName() + "\r\n");
	// server.sendToClient(client.getFd(), RPL_TOPIC(client.getNickname(), commands[1], it->second->getTopic()));
	// server.sendToClient(client.getFd(), RPL_NAMREPLY(client.getNickname(), commands[1], it->second->getUsers()));
	// server.sendToClient(client.getFd(), RPL_ENDOFNAMES(client.getNickname(), commands[1]));
	// server.sendToClient(client.getFd(), RPL_MOTDSTART(client.getNickname()));
	// server.sendToClient(client.getFd(), RPL_MOTD(client.getNickname()));
	// server.sendToClient(client.getFd(), RPL_ENDOFMOTD(client.getNickname()));
}


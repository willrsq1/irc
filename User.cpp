#include "Commands.hpp"

void	user(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (client.getIsRegistered() == true)
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		return ;
	}

	if (client.getHasSetNickname() == false)
	{
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return ;
	}

	if (commands.size() < 5 || commands[1].empty())
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
		return ;
	}

	if (commands.size() > 5)
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

	if (commands[1].size() > USERLEN)
	{
		commands[1].resize(USERLEN);
	}

// parsing des caracs autorisés ?

	if (commands[2] != "0" || commands[3] != "*")
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("The second parrameter must be '0', the third parameter must be '*'"));
		return ;
	}

	commands[1] = "~" + commands[1];

	client.setUsername(commands[1]);
	client.setRealname(commands[4]);
	client.setIsRegistered(true);


	server.sendToClient(client.getFd(), RPL_WELCOME(client.getNickname(), client.getUsername()));
	server.sendToClient(client.getFd(), RPL_YOURHOST(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_CREATED(client.getNickname(), server.getCreationDate()));
	server.sendToClient(client.getFd(), RPL_MYINFO(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_ISUPPORT(client.getNickname()));

}
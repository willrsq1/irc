#include "Commands.hpp"

void	user(Server & server, Client & client, std::vector<std::string> & commands)
{

	if (client.getIsRegistered() == true) // already registered
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getNickname()));
		return ;
	}

	if (client.getHasSetNickname() == false) // nickname not set yet
	{
		server.sendToClient(client.getFd(), ERR_NONICKNAMEGIVEN(client.getNickname()));
		return ;
	}

	if (commands.size() < 5 || commands[1].empty()) // parameters insufficient
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
		return ;
	}

	if (commands.size() > 5) // too many parameters
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("Too many parameters"));
		return ;
	}

// parsing des caracs autorisés ?

	if (commands[2] != "0" || commands[3] != "*") // second and third parameters must be "0" and "*"
	{
		server.sendToClient(client.getFd(), ERR_FATALERROR("The second parameter must be '0', the third parameter must be '*'"));
		return ;
	}

	commands[1] = "~" + commands[1]; // ~ is added to the username because the server is not using IDENT PROTOCOL (https://datatracker.ietf.org/doc/html/rfc1413)

	if (commands[1].size() > USERLEN) // username too long
	{
		commands[1].resize(USERLEN);
		server.sendToClient(client.getFd(), ERR_FATALERROR("Your username was resized to: " + commands[1]));
	}

	client.setUsername(commands[1]);
	client.setRealname(commands[4]);

	client.setIsRegistered(true); //ready to use the server !

	//send welcome messages

	server.sendToClient(client.getFd(), RPL_WELCOME(client.getNickname(), client.getUsername()));
	server.sendToClient(client.getFd(), RPL_YOURHOST(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_CREATED(client.getNickname(), server.getCreationDate()));
	server.sendToClient(client.getFd(), RPL_MYINFO(client.getNickname()));
	server.sendToClient(client.getFd(), RPL_ISUPPORT(client.getNickname()));

}
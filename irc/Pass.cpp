#include "Commands.hpp"

void	pass(Server & server, Client & client, std::vector<std::string> & commands)
{
	if (client.getIsRegistered() == true)
	{
		server.sendToClient(client.getFd(), ERR_ALREADYREGISTRED(client.getName()));
		return ;
	}

	if (commands.size() != 2)
	{
		server.sendToClient(client.getFd(), ERR_NEEDMOREPARAMS(client.getName(), "PASS"));
		client.setHasPassword(false);
		return ;
	}

	if (commands[1] != server.getPassword())
	{
		server.sendToClient(client.getFd(), ERR_PASSWDMISMATCH(client.getName()));
		server.sendToClient(client.getFd(), ERR_FATALERROR("Wrong password, goodbye ! :) We are disconnecting you."));
		server.disconnectClient(client);
		return ;
	}
	client.setHasPassword(true);
}
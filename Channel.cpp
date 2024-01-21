#include "Channel.hpp"

Channel::Channel(std::string name, Server * server): name(name), server(server)
{

}

Channel::~Channel()
{
	
}

void Channel::addClient(int fd, Client * client)
{
	if (isBanned(fd))
	{
		server->sendToClient(fd, ERR_FATALERROR("You are banned from this channel."));
		return ;
	}
	clients[fd] = client;
	if (operators.size() == 0)
		setNewOperator(fd);
}

bool Channel::isClientInChannel(int fd)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{
		
		if ((*it).second->getFd() == fd)
			return (true);

	}
	return (false);
}

std::string & Channel::getName()
{
	return (this->name);
}

void Channel::sendToAllClients(std::string const & message)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{

		server->sendToClient((*it).second->getFd(), message);

	}
}

void Channel::sendToAllClientsExceptOne(int fd, std::string const & message)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{
		
		if (fd == (*it).second->getFd())
			continue ;
		server->sendToClient((*it).second->getFd(), message);

	}
}

void Channel::addOperator(int fd)
{
	this->operators.push_back(fd);
}

void Channel::removeOperator(int fd)
{
	for (std::vector<int>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{

		if (*it == fd)
		{
			this->operators.erase(it);
			return ;
		}

	}
}

bool Channel::isOperator(int fd)
{
	for (std::vector<int>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{

		if (*it == fd)
			return (true);

	}
	return (false);
}

void Channel::removeClient(int fd)
{
	for (it_clients it = this->clients.begin(); it != this->clients.end(); it++)
	{

		if ((*it).second->getFd() == fd)
		{
			if (this->isOperator(fd))
				this->operators.erase(std::find(this->operators.begin(), this->operators.end(), fd));
			this->clients.erase(it);
			break ;
		}

	}

	if (this->clients.size() == 0)
	{
		server->removeChannel(this->name);
		delete this;
		return ;
	}

	if (this->operators.size() == 0)
	{
		this->setNewOperator(this->clients.begin()->second->getFd());
	}
}

void Channel::setNewOperator(int fd)
{
	this->operators.push_back(fd);
	server->sendToClient(fd, "you are now operator of this channel\r\n");
	// server->sendToClient(this->clients.begin()->second->getFd(), MODE(this->clients.begin()->second->getNickname(), this->name, "+o", nickname));
}

void Channel::addBannedUser(int fd)
{
	this->bannedUsers.push_back(fd);
}

bool Channel::isBanned(int fd)
{
	for (std::vector<int>::iterator it = bannedUsers.begin(); it != bannedUsers.end(); it++)
	{
		if (*it == fd)
			return (true);
	}
	return (false);
}
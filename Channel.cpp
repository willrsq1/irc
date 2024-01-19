#include "Channel.hpp"

Channel::Channel(std::string name, Server * server): name(name), server(server)
{

}

Channel::~Channel()
{
	
}

void Channel::addClient(int fd, Client * client)
{
	this->clients[fd] = client;
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

void Channel::addOperator(std::string const & nickname)
{
	this->operators.push_back(nickname);
}

void Channel::removeOperator(std::string const & nickname)
{
	for (std::vector<std::string>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		if (*it == nickname)
		{
			this->operators.erase(it);
			return ;
		}
	}
}

bool Channel::isOperator(std::string const & nickname)
{
	for (std::vector<std::string>::iterator it = this->operators.begin(); it != this->operators.end(); it++)
	{
		if (*it == nickname)
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
			this->clients.erase(it);
			break ;
		}
	}

	if (this->clients.size() == 0)
	{
		// server->removeChannel(this->name);
		delete this;
		return ;
	}

	if (this->operators.size() == 0)
	{
		this->setNewOperator(this->clients.begin()->second->getNickname());
	}
}

void Channel::setNewOperator(std::string const & nickname)
{
	this->operators.push_back(nickname);
	server->sendToClient(this->clients.begin()->second->getFd(), "you are now operator of this channel");
	// server->sendToClient(this->clients.begin()->second->getFd(), MODE(this->clients.begin()->second->getNickname(), this->name, "+o", nickname));
}
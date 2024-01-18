#include "Client.hpp"

Client::Client(): hasPassword(false), name("NO_NICK"), isRegistered(false), iPoll(0)
{

}

Client::~Client()
{
	close(this->my_fd);
}

void	Client::setFd(int fd)
{
	this->my_fd = fd;
	if (name == "NO_NICK")
	{
		this->name = "Anonymous";
		std::ostringstream ss;
		ss << (fd - 3);
		this->name += ss.str();
	}
}

std::string & Client::getSocketBuff()
{
	return (this->socketBuff);
}

bool	Client::getHasPassword()
{
	return (this->hasPassword);
}

std::string & Client::getName()
{
	return (this->name);
}

void	Client::setHasPassword(bool hasPassword)
{
	this->hasPassword = hasPassword;
}

void Client::setIsRegistered(bool isRegistered)
{
	this->isRegistered = isRegistered;
}

bool Client::getIsRegistered()
{
	return (this->isRegistered);
}

void Client::setName(std::string & name)
{
	this->name = name;
}

void Client::setiPoll(int iPoll)
{
	this->iPoll = iPoll;
}

int Client::getiPoll()
{
	return (this->iPoll);
}

int Client::getFd()
{
	return (this->my_fd);
}
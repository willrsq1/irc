#include "Client.hpp"

Client::Client() {};

Client::Client(int fd): hasPassword(false), hasSetNickname(false), isRegistered(false), nickname("NO_NICK"), iPoll(0)
{
	this->my_fd = fd;
	this->nickname = "Anonymous";
	std::ostringstream ss;
	ss << (fd - 3);
	this->nickname += ss.str();
	realname = nickname;
	username = nickname;
}

Client::~Client()
{
	close(this->my_fd);
}

void	Client::setFd(int fd)
{
	this->my_fd = fd;
}

std::string & Client::getSocketBuff()
{
	return (this->socketBuff);
}

bool	Client::getHasPassword()
{
	return (this->hasPassword);
}

std::string & Client::getNickname()
{
	return (this->nickname);
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

void Client::setNickname(std::string & nickname)
{
	this->nickname = nickname;
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

std::string & Client::getUsername()
{
	return (this->username);
}

void Client::setUsername(std::string & username)
{
	this->username = username;
}

std::string & Client::getRealname()
{
	return (this->realname);
}

void Client::setRealname(std::string & realname)
{
	this->realname = realname;
}

void Client::setHasSetNickname(bool hasSetNickname)
{
	this->hasSetNickname = hasSetNickname;
}

bool Client::getHasSetNickname()
{
	return (this->hasSetNickname);
}
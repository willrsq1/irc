#ifndef CLIENT_CPP
# define CLIENT_CPP

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <map>

class Channel;

typedef	typename std::map<std::string, Channel *>::iterator it_channels;

class Client
{
	private:

		std::map<std::string, Channel *> channels;

		bool 			hasPassword;
		bool			hasSetNickname;
		bool 			isRegistered;
		std::string		nickname;
		std::string		username;
		std::string		realname;
		int				iPoll;
		int				my_fd;

		std::string		socketBuff;
		Client();

	public:
		Client(int fd);
		~Client();


		bool	getHasPassword();
		bool	getHasSetNickname();
		bool	getIsRegistered();
		std::string & getNickname();
		std::string & getUsername();
		std::string & getRealname();
		int		getiPoll();
		int		getFd();
		std::string & getSocketBuff();

		void	setHasPassword(bool);
		void	setHasSetNickname(bool);
		void	setIsRegistered(bool);
		void	setNickname(std::string &);
		void	setUsername(std::string &);
		void	setRealname(std::string &);
		void	setiPoll(int);
		void	setFd(int);

		void	addChannel(std::string const & channelName, Channel * channel);
		void	removeChannel(std::string const & channelName);
};

#include "Channel.hpp"

#endif
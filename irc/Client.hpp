#ifndef CLIENT_CPP
# define CLIENT_CPP

#include <iostream>
#include <unistd.h>
#include <sstream>

class Client
{
	private:

		bool 			hasPassword;
		std::string		name;
		bool 			isRegistered;
		int				iPoll;

		int				my_fd;
		std::string		socketBuff;

	public:
		Client();
		~Client();


		std::string & getSocketBuff();
		std::string & getName();
		bool	getHasPassword();
		bool	getIsRegistered();
		int		getiPoll();
		int		getFd();

		void	setFd(int);
		void	setiPoll(int);
		void	setName(std::string &);
		void	setHasPassword(bool);
		void	setIsRegistered(bool);
};

#endif
#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include "sys/socket.h"
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "NumericReplies.hpp"

#define BUFFER_SIZE 4096
#define HOST "localhost"

class Client;

typedef	typename std::map<int, Client *>::iterator 	it_clients;

class Server
{
	typedef void (*fct)(Server &, Client &, std::vector<std::string> &);

	private:

		std::map<std::string, fct> 	commands;

		std::map<int, Client *> 	clients;


		pollfd 				pollfds[SOMAXCONN];
		std::string 		password;
		int					nbSockets;

		Server();
		void loop();
		void createMySocket(int port);
		void newClient();
		void newMessage(int i);
		void commandExecution(std::string & command, int i);
		void registerCommand();


	public:
		
		Server(std::string const & port, std::string const & password);
		~Server();
		void disconnectClient(Client & client);
		std::string getPassword();
		void sendToClient(int fd, std::string const & message);

		it_clients getClientsMapEnd();
		it_clients getClientsMapBegin();
		
};

#include "Client.hpp"
#include "Commands.hpp"

#endif
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
#include <signal.h>

#include "NumericReplies.hpp"

#define BUFFER_SIZE 4096
#define HOST "localhost"
#define SERVER "ARBESA_SERVER"
#define USERLEN 20
#define USERMODE "jspUSERMODE"
#define CHANMODE "jspCHANMODE"
#define CHANLIMIT 5

class Client;
class Channel;


typedef	typename std::map<int, Client *>::iterator 	it_clients;
typedef	typename std::map<std::string, Channel *>::iterator it_channels;

class Server
{
	typedef void (*fct)(Server &, Client &, std::vector<std::string> &);

	private:

		bool 					shutting_down;

		std::map<std::string, fct> 	commands;

		std::map<int, Client *> 	clients;

		std::map<std::string, Channel *> 	channels;

		pollfd 				pollfds[SOMAXCONN];
		std::string 		password;
		int					nbSockets;

		std::string 		creationDate;

		Server();
		void loop();
		void createMySocket(int port);
		void newClient();
		void newMessage(int i);
		void commandExecution(std::string & command, int i);
		void registerCommand();
		void registerDateCreation();


	public:
		
		Server(std::string const & port, std::string const & password);
		~Server();
		void disconnectClient(Client & client);
		std::string getPassword();
		void sendToClient(int fd, std::string const & message);
		void sendToAllClients(std::string const & message);
		void sendToAllClientsInChannel(std::string const & channelName, std::string const & message);
		std::string getCreationDate();

		it_clients  getClientsBegin();
		it_clients  getClientsEnd();

		it_channels getChannelsBegin();
		it_channels getChannelsEnd();

		void addChannel(std::string const & channelName, Channel * channel);
		void removeChannel(std::string const & channelName);

		void removeClientFromChannel(std::string const & channelName, int fd);

		static void mySigIntHandler(int s);
		static bool 				running;
		
};

#include "Client.hpp"
#include "Channel.hpp"
#include "Commands.hpp"

#endif
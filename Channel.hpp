#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <map>
#include <iostream>
#include <vector>

class Client;
class Server;

typedef	typename std::map<int, Client *>::iterator 	it_clients;

class Channel
{
	private:

		std::map<int, Client *> clients;
		std::vector<std::string> operators;
		std::string name;

		Server * server;

		Channel();

	public:
		Channel(std::string name, Server * server);
		~Channel();

		std::string & getName();



		//OPERATORS
		void addOperator(std::string const & nickname);
		void removeOperator(std::string const & nickname);
		bool isOperator(std::string const & nickname);
		void setNewOperator(std::string const & nickname);



		bool isClientInChannel(int fd);
		void sendToAllClients(std::string const & message);
		void addClient(int fd, Client * client);
		void removeClient(int fd);
};

#include "Client.hpp"
#include "Server.hpp"
#endif
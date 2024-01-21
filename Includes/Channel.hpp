#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <map>
#include <iostream>
#include <vector>

class Client;
class Server;

typedef	std::map<int, Client *>::iterator 	it_clients;

class Channel
{
	private:

		std::map<int, Client *> clients;
		std::vector<int> operators;
		std::vector<int> bannedUsers;
		std::string name;
		std::string topic;
		Server * server;
		std::string mode;

		bool limit;
		int limitValue;

		Channel();

	public:
		Channel(std::string name, Server * server);
		~Channel();

		std::string & getName();



		//OPERATORS
		void addOperator(int fd);
		void removeOperator(int fd);
		bool isOperator(int fd);
		void setNewOperator(int fd);



		bool isClientInChannel(int fd);
		void sendToAllClients(std::string const & message);
		void sendToAllClientsExceptOne(int fd, std::string const & message);
		void addClient(int fd, Client * client);
		void removeClient(int fd);

		int getNbClients();
		std::string & getTopic();


		void addBannedUser(int fd);
		void removeBannedUser(int fd);
		bool isBanned(int fd);


		std::string & getMode();
		bool isSetMode(char c);
		void setMode(char c);
		void unsetMode(char c);
		void setLimitValue(int limitValue);
		int getLimitValue();
		bool limitIsReached();
		void setLimit(bool limit);
};

#include "Client.hpp"
#include "Server.hpp"
#endif
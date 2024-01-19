#include "Server.hpp"

//je code en c++98

Server::Server(){}

bool Server::running = true;

Server::Server(std::string const & port, std::string const & password): password(password), nbSockets(0)
{
	for (size_t i = 0; i < port.size(); i++)
	{
		if (port[i] < '0' || port[i] > '9')
			throw std::runtime_error("Error: port must be a number");
	}

	int port_nb = atoi(port.c_str());

	if (port_nb != 6667)
		throw std::runtime_error("Error: port must be 6667 for TCP connections");
	
	registerDateCreation();
	registerCommand();
	createMySocket(port_nb);
	loop();
}

void	Server::registerCommand()
{
	commands["PASS"] = &pass;
	commands["NICK"] = &nick;
	commands["USER"] = &user;
	commands["PRIVMSG"] = &privmsg;
	commands["JOIN"] = &join;
	// functions["/join"] = &join;
	// functions["/leave"] = &leave;
	// functions["/msg"] = &msg;
	// functions["/accept_file"] = &accept_file;
	// functions["/send_file"] = &send_file;
	// functions["/quit"] = &quit;
}

Server::~Server()
{
	for (int i = 0; i < nbSockets; i++)
	{
		close(pollfds[i].fd);
	}

	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		delete it->second;
	}
	
	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		delete it->second;
	}
}

void Server::createMySocket(int port)
{
	int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1)
		throw std::runtime_error("Error: socket creation failed");

	int yes = 1;
	if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw std::runtime_error("Error: setsockopt(SO_REUSEADDR) failed");
	
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(serverSocketFd, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::runtime_error("Error: bind failed");

	if (listen(serverSocketFd, SOMAXCONN) == -1)
		throw std::runtime_error("Error: listen failed");

	pollfds[nbSockets].fd = serverSocketFd;
	pollfds[nbSockets].events = POLLIN;
	nbSockets++;
	std::cout << "Server is listening on port " << port << std::endl;
	
}

void	Server::loop()
{
	while (running == true)
	{
		int ret = poll(pollfds, nbSockets, 1000);
		if (ret == -1)
		{
			if (running == false)
				break ;
			throw std::runtime_error("Error: poll failed");
		}
		else if (ret == 0)
			continue ;
		for (int i = 0; i < nbSockets; i++)
		{
			if ((pollfds[i].revents & POLLIN) != 0)
			{
				//fct qui check erreur de revent
				if (i == 0)
				{
					newClient();
				}
					//serv
				else
				{
					newMessage(i);
				}
					//client
			}
		}
	}
	std::cerr << "Server is shutting down" << std::endl;
}



void	Server::newClient()
{
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	int clientSocketFd = accept(pollfds[0].fd, (sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocketFd == -1)
		throw std::runtime_error("Error: accept failed");
	
	pollfds[nbSockets].fd = clientSocketFd;
	pollfds[nbSockets].events = POLLIN;
	pollfds[nbSockets].revents = 0;


	clients[clientSocketFd] = new Client(clientSocketFd);
	clients[clientSocketFd]->setiPoll(nbSockets);
	std::cout << "New client connected :" << clients[clientSocketFd]->getNickname() << std::endl;
	nbSockets++;
}



void	Server::newMessage(int i)
{
	char	buff[BUFFER_SIZE];
	std::string & socketBuff = clients[pollfds[i].fd]->getSocketBuff();

	int ret = recv(pollfds[i].fd, buff, sizeof(buff), 0);
	if (ret == -1)
		throw std::runtime_error("Error: recv failed");
	if (ret == 0)
	{
		disconnectClient(*clients[pollfds[i].fd]);
		return ;
	}
	else if (ret == 512)
	{
		sendToClient(pollfds[i].fd, ERR_INPUTTOOLONG(clients[pollfds[i].fd]->getNickname()));
		return ;
	}
	buff[ret] = '\0';

	socketBuff += buff;


	size_t pos = socketBuff.find("\r\n");
	if (pos == std::string::npos)
		pos = socketBuff.find("\n");
	while (pos != std::string::npos)
	{
		std::string command = socketBuff.substr(0, pos);
		socketBuff.erase(0, pos + 2);
		commandExecution(command, pollfds[i].fd);
		if (clients.find(pollfds[i].fd) == clients.end())
			return ;
		pos = socketBuff.find("\r\n");
		if (pos == std::string::npos)
			pos = socketBuff.find("\n");
	}
	
}

void	Server::commandExecution(std::string & command, int clientSocket)
{
	std::vector<std::string> tokens = split(command, ' ');

	if (tokens.size() > 0 && commands.find(tokens[0]) != commands.end())
	{
		if (tokens[0] != "PASS" && clients[clientSocket]->getHasPassword() == false)
		{
			sendToClient(clientSocket, ERR_FATALERROR("You must send connect with the right password first"));
		}
		else if ((*clients[clientSocket]).getIsRegistered() == false && tokens[0] != "PASS" && tokens[0] != "NICK" && tokens[0] != "USER")
		{
			sendToClient(clientSocket, ERR_NOTREGISTERED());
		}
		else
		{
			commands[tokens[0]](*this, *clients[clientSocket], tokens);
		}
	}
	std::cout << "Message from client nb [" << clientSocket - 3 << "] : " << command << std::endl;

}

void	Server::sendToClient(int fd, std::string const & message)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: send failed");
	std::cout << (std::string)BLUE << "SENT TO CLIENT [" << fd - 3 << "] : " << message << NOCOLOR;
}

void	Server::disconnectClient(Client & client)
{
	int i = client.getiPoll();

	std::cout << "Client number [" << client.getFd() - 3 << "] disconnected." << std::endl;

	pollfds[i] = pollfds[nbSockets - 1]; //move last client to the empty slot
	clients[pollfds[i].fd]->setiPoll(i); //update the index of the client in the pollfds array
	

	clients.erase(client.getFd());
	delete &client;
	nbSockets--;

}





std::string Server::getPassword()
{
	return password;
}

it_clients Server:: getClientsBegin()
{
	return clients.begin();
}

it_clients Server:: getClientsEnd()
{
	return clients.end();
}

it_channels Server::getChannelsBegin()
{
	return channels.begin();
}

it_channels Server::getChannelsEnd()
{
	return channels.end();
}

void Server::registerDateCreation()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", timeinfo);
	creationDate = buffer;
}

std::string Server::getCreationDate()
{
	return creationDate;
}

void Server::mySigIntHandler(int s)
{
	std::cout << std::endl << "Caught signal " << s << std::endl;
	Server::running = false;
}

void Server::sendToAllClients(std::string const & message)
{
	for (it_clients it = clients.begin(); it != clients.end(); it++)
	{
		sendToClient(it->second->getFd(), message);
	}
}

void Server::sendToAllClientsInChannel(std::string const & channelName, std::string const & message)
{
	for (it_channels it = channels.begin(); it != channels.end(); it++)
	{
		if (it->first == channelName)
		{
			it->second->sendToAllClients(message);
			return ;
		}
	}
}

void Server::addChannel(std::string const & channelName, Channel * channel)
{
	channels[channelName] = channel;
}
#include "Bot.hpp"

Bot::Bot(): nbWinners(0), questionNb(0), questionsTotalNb(4)
{
	lastUpdate = time(NULL);
	mode = 0;

	questions[0] = "What is the answer to life, the universe and everything ?";
	answers[0] = "42";

	questions[1] = "What is the capital of France ?";
	answers[1] = "Paris";

	questions[2] = "What is the capital of Germany ?";
	answers[2] = "Berlin";

	questions[3] = "Where is the Moon ?";
	answers[3] = "Space";
}

Bot::~Bot()
{

}

void Bot::setServer(Server * server)
{
	this->server = server;
}

void	Bot::update()
{
	if (server->noRegisteredClients() == true)
	{
		lastUpdate = time(NULL);
		mode = 0;
		nbWinners = 0;
		winners.clear();
		return ;
	}
	if (time(NULL) - lastUpdate < 30)
	{
		return ;
	}
	lastUpdate = time(NULL);
	if (mode == 0)
	{
		// server->sendToAllClientsFromBot("Hello everyone ! I'm Jack, I am the bot of this server.\r\n");
		// server->sendToAllClientsFromBot("I will ask you a question, the first one to answer correctly will win 10 points !\r\n");
		// server->sendToAllClientsFromBot((std::string)"You can see your points by sending me a private message with the command: 'PRIVMSG " + BOT_NAME + " MY_POINTS'\r\n");
		server->sendToAllClientsFromBot(PRIVMSG(BOT_NAME, BOT_NAME, SERVER, "Please answer this question correctly to win points: " + questions[questionNb]));
		// server->sendToAllClientsFromBot((std::string)"To answer the question, just send me a private message with the answer: 'PRIVMSG " + BOT_NAME + " <answer>' \r\n");
		// server->sendToAllClientsFromBot("The winners will be named in 90 seconds !\r\n");
		mode++;
		return ;
	}
	else if (mode == 1)
	{
		server->sendToAllClientsFromBot(PRIVMSG(BOT_NAME, BOT_NAME, SERVER, "Only 60 seconds remaining to answer the question !"));
		mode++;
		return ;
	}
	else if (mode == 2)
	{
		server->sendToAllClientsFromBot(PRIVMSG(BOT_NAME, BOT_NAME, SERVER, "Only 30 seconds remaining to answer the question... Hurry up !"));
		mode++;
		return ;
	}
	else if (mode == 3)
	{
		if (nbWinners == 0)
		{
			server->sendToAllClientsFromBot(PRIVMSG(BOT_NAME, BOT_NAME, SERVER, (std::string)"No one answered correctly... Too bad ! The right answer was: " + answers[questionNb]));
		}
		else
		{
			std::string winners = getWinners();
			server->sendToAllClientsFromBot(PRIVMSG(BOT_NAME, BOT_NAME, SERVER, (std::string)"Congratulations to the winner(s): " + winners + "!"));
		}
		mode = 0;
		nbWinners = 0;
		questionNb++;
		if (questionNb == questionsTotalNb)
			questionNb = 0;
		winners.clear();
		return ;
	}


}

std::string Bot::getWinners()
{
	std::string winners_names;

	for (std::vector<int>::iterator it = winners.begin(); it != winners.end(); ++it)
	{
		for (it_clients it2 = server->getClientsBegin(); it2 != server->getClientsEnd(); ++it2)
		{
			if (it2->second->getFd() == *it)
			{
				winners_names += it2->second->getNickname();
				if (nbWinners > 1)
					winners_names += ", ";
				else
					winners_names += " ";
				nbWinners--;
				server->sendToClient(it2->second->getFd(), PRIVMSG(BOT_NAME, BOT_NAME, it2->second->getNickname(), "Congratulations ! You were one of the Winner(s) ! You win 10 points."));
				it2->second->addBotPoints(10);
			}
		}
	}
	return (winners_names);
}

void	Bot::receiveAnswer(int fd, std::string const & answer)
{

	Client * client = server->getClientFromFd(fd);

	if (answer == "MY_POINTS")
	{
		std::ostringstream oss;
		oss << client->getBotPoints();
		std::string result = oss.str();
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), (std::string)"You have " + result + " points."));
		return ;
	}
	else if (answer == "ENABLE")
	{
		if (client->getBotEnabled() == true)
		{
			server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Bot already enabled."));
			return ;
		}
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Bot enabled."));
		client->setBotEnabled(true);
		return ;
	}
	else if (answer == "DISABLE")
	{
		if (client->getBotEnabled() == false)
		{
			server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Bot already disabled."));
			return ;
		}
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Bot disabled."));
		client->setBotEnabled(false);
		return ;
	}
	else if (answer == "HELP")
	{
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Commands: MY_POINTS, ENABLE, DISABLE, HELP"));
		return ;
	}

	if (client->getBotEnabled() == false)
	{
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, client->getNickname(), "Bot disabled. You can enable it with the command: 'PRIVMSG " + BOT_NAME + " ENABLE'"));
		return ;
	}
	std::string nickname = client->getNickname();

	for (std::vector<int>::iterator it = winners.begin(); it != winners.end(); ++it)
	{
		if (*it == fd)
		{
			server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, nickname, "You already answered correctly !"));
			return ;
		}
	}
	if (answer == answers[questionNb])
	{
		nbWinners++;
		winners.push_back(fd);
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, nickname, "Congratulations ! You found the right answer !"));
	}
	else
	{
		server->sendToClient(fd, PRIVMSG(BOT_NAME, BOT_NAME, nickname ,"Wrong answer !"));
	}
}
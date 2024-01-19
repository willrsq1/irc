#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

# define BLUE        "\033[1;34m"
# define NOCOLOR     "\033[0m"

//x

#define RPL_NEWNICKNAME(client, nickname) (client + " changed her nickname to " + nickname + "\r\n")

//x

#define ERR_FATALERROR(msg) ((std::string)"ERROR : Fatal error: " + msg + "\r\n")

#define SRC_CLI(client, username) (client + "!" + username + "@" + HOST)

#define PRIVMSG(client, username, target, message) ((std::string)":" + SRC_CLI(client, username) + " PRIVMSG " + target + " :" + message + "\r\n")



//001
#define RPL_WELCOME(client, username) ((std::string)":" + SERVER + " 001 " + client + " :Welcome to the " + SERVER + " Network, " + SRC_CLI(client, username) + "\r\n")
//002
#define RPL_YOURHOST(client) ((std::string)":" + SERVER + " 002 " + client + " :Your host is " + SERVER + ", running version 1.0\r\n")
//003
#define RPL_CREATED(client, datetime) ((std::string)":" + SERVER + " 003 " + client + " :This server was created " + datetime + "\r\n")
//004
#define RPL_MYINFO(client) ((std::string)":" + SERVER + " 004 " + client + " " + SERVER + " version 1.0 " + USERMODE + " " + CHANMODE + "\r\n")
//005
#define RPL_ISUPPORT(client) ((std::string)":" + SERVER + " 005 " + client + " PREFIX=(ov)@+ CHANTYPES=#&+ CHANMODES=" + CHANMODE + " NICKLEN=20 TOPICLEN=50 KICKLEN=50 MODES=3 NETWORK=" + SERVER + " CASEMAPPING=ascii :are supported by this server\r\n")



//401
#define ERR_NOSUCHNICK(client, target) ((std::string)":" + SERVER + " 401 " + client + " " + target + " :No such nick/channel\r\n")

//403
#define ERR_NOSUCHCHANNEL(client, channel) ((std::string)":" + SERVER + " 403 " + client + " " + channel + " :No such channel\r\n")

//411
#define ERR_NORECIPIENT(client, command) ((std::string)":" + SERVER + " 411 " + client + " :No recipient given " + command + "\r\n")
//412
#define ERR_NOTEXTTOSEND(client) ((std::string)":" + SERVER + " 412 " + client + " :No text to send\r\n")


//417
#define ERR_INPUTTOOLONG(client) ((std::string)":" + SERVER + " 417 " + client + " :Input line was too long\r\n")

//NICK
//431
#define ERR_NONICKNAMEGIVEN(client) ((std::string)":" + SERVER + " 431 " + client + " :No nickname given\r\n")
//432
#define ERR_ERRONEUSNICKNAME(client, nickname, msg) ((std::string)":" + SERVER + " 432 " + client + " " + nickname + " :" + msg + "\r\n")
//433
#define ERR_NICKNAMEINUSE(client, nickname) ((std::string)":" + SERVER + " 433 " + client + " " + nickname + " :Nickname is already in use\r\n")

//451
#define ERR_NOTREGISTERED(client) ((std::string)":" + SERVER + " 451 " + client + " :You have not registered\r\n")

//PASS
// 461
#define ERR_NEEDMOREPARAMS(client, command) ((std::string)":" + SERVER + " 461 " + client + " " + command + " :Wrong number of parameters\r\n")
//462
#define ERR_ALREADYREGISTRED(client) ((std::string)":" + SERVER + " 462 " + client + " :You may not reregister\r\n")
//464
#define ERR_PASSWDMISMATCH(client) ((std::string)":" + SERVER + " 464 " + client + " :Password incorrect\r\n")



#endif
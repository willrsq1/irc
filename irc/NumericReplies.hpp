#ifndef NUMERICREPLIES_HPP
# define NUMERICREPLIES_HPP

//x

#define RPL_NEWNICKNAME(client, nickname) (client + " changed her nickname to " + nickname + "\r\n")

//x

#define ERR_FATALERROR(msg) ((std::string)"ERROR : Fatal error: " + msg + "\r\n")

//NICK
//431
#define ERR_NONICKNAMEGIVEN(client) (client + " :No nickname given\r\n")
//432
#define ERR_ERRONEUSNICKNAME(client, nickname, msg) (client + " " + nickname + " :" + msg + "\r\n")
//433
#define ERR_NICKNAMEINUSE(client, nickname) (client + " " + nickname + " :Nickname is already in use\r\n")

//451
#define ERR_NOTREGISTERED(client) (client + " :You have not registered\r\n")

//PASS
// 461
#define ERR_NEEDMOREPARAMS(client, command) (client + " " + command + " :Wrong number of parameters\r\n")
//465
#define ERR_PASSWDMISMATCH(client) (client + " :Password incorrect\r\n")
//462
#define ERR_ALREADYREGISTRED(client) (client + " :You may not reregister\r\n")



#endif
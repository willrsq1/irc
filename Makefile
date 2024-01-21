### COMPILATION ###

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98

### EXECUTABLE ###

NAME    = ircserv

### SOURCE FILES ###

SOURCES =	Sources/main.cpp \
			Sources/Server.cpp \
			Sources/Client.cpp \
			Sources/Channel.cpp \
			Commands/Utils.cpp \
			Commands/Join.cpp \
			Commands/Pass.cpp \
			Commands/Nick.cpp \
			Commands/User.cpp \
			Commands/Privmsg.cpp \
			Commands/Part.cpp \
			Commands/Kick.cpp \
			Commands/Motd.cpp \
			Commands/List.cpp \
			Commands/Ping.cpp \
			Commands/Quit.cpp \
			Sources/Bot.cpp \
			
OBJS    = $(SOURCES:.cpp=.o)

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

%.o: %.cpp %.hpp %/%.hpp
	$(CC) $(FLAGS) -c -o $@ $<
	
clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re template

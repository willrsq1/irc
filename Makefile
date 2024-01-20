### COMPILATION ###

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98

### EXECUTABLE ###

NAME    = ircserv

### SOURCE FILES ###

SOURCES =	main.cpp \
			Server.cpp \
			Client.cpp \
			Channel.cpp \
			Utils.cpp \
			Join.cpp \
			Pass.cpp \
			Nick.cpp \
			User.cpp \
			Privmsg.cpp \
			Part.cpp \
			Bot.cpp \
			
OBJS    = $(SOURCES:.cpp=.o)

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

%.o: %.cpp *.hpp
	$(CC) $(FLAGS) -c -o $@ $<
	
clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re template

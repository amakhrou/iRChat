NAME = ircserv

CPPFLAGS = c++ -Wall -Wextra -Werror

SRC =   conf/configFile.cpp  main.cpp multuplixing/multuplixing.cpp \
		parse/parse.cpp Server.cpp irc.cpp

all : $(NAME) $(SRC) 

$(NAME) : $(SRC)	
	$(CPPFLAGS) $(SRC) -o $(NAME)
clean : 
		rm -fr  $(NAME)

fclean : clean
		rm -fr $(NAME) 

re : clean all
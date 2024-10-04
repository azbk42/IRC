# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 14:57:32 by ctruchot          #+#    #+#              #
#    Updated: 2024/10/04 12:34:03 by ctruchot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = IRC

# SRC = irc.cpp
# SRC = main.cpp
# SRC = poll2.cpp
# SRC = test_socket.cpp \
# 	client.cpp \

SRC = main.cpp \
	server.cpp

	
CC		= c++
# CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c++98
# CFLAGS = -Wall -Werror -Wextra


OBJ = $(SRC:%.cpp=%.o)
# OBJ = $(SRC:%.c=%.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean 
	make all

.PHONY: clean all fclean re all so
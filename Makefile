# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ctruchot <ctruchot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 14:57:32 by ctruchot          #+#    #+#              #
#    Updated: 2024/10/04 14:50:46 by ctruchot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = IRC

SRC = main.cpp \
	server.cpp

	
CC		= c++
CFLAGS = -Wall -Werror -Wextra -std=c++98

OBJ = $(SRC:%.cpp=%.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean 
	make all

.PHONY: clean all fclean re all so
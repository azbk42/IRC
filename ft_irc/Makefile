################################################################################
#                                COMPILATION                                   #
################################################################################

NAME = ircserv
BONUS_NAME = ircbot
CC = c++
CFLAGS = -g3 -Werror -Wall -Wextra -std=c++98 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/class/cmd -I$(INCLUDE_DIR)/class
MAKEFLAGS += --no-print-directory

################################################################################
#                                  SOURCES                                     #
################################################################################

SRC_DIR = src
CLASS_DIR = $(SRC_DIR)/class
CMD_DIR = $(CLASS_DIR)/cmd
INCLUDE_DIR = ./includes
BONUS_DIR = ./bonus

# fichier in src/class
SRC_CLASS = server.cpp client.cpp parse.cpp channel.cpp

SRC_CMD = join.cpp pass.cpp nick.cpp quit.cpp list.cpp part.cpp msg.cpp whois.cpp mode.cpp mode2.cpp kick.cpp topic.cpp invite.cpp
# fichier in src/
SRC_FILES = handle_arg.cpp utils.cpp

SRC = $(addprefix $(CLASS_DIR)/, $(SRC_CLASS)) \
      $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
	  $(addprefix $(CMD_DIR)/, $(SRC_CMD)) \
		main.cpp

# bonus sources
BONUS_SRC = $(BONUS_DIR)/main.cpp $(BONUS_DIR)/bot.cpp

################################################################################
#                                  OBJECTS / DEP                               #
################################################################################

OBJ_DIR = obj
OBJ := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
BONUS_OBJ := $(BONUS_SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS := $(OBJ:.o=.d)
BONUS_DEPS := $(BONUS_OBJ:.o=.d)

################################################################################
#                                  COLOR                                       #
################################################################################

PINK = \033[1;95m
RED = \033[0;91m
CYAN = \033[0;96m
MAGENTA = \033[1;35m
RESET = \033[0m

################################################################################
#                                  MAKEFILE                                    #
################################################################################

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\n$(CYAN)$(NAME) compiled!$(RESET)"

$(BONUS_NAME): $(BONUS_OBJ)
	@$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(BONUS_NAME)
	@echo "\n$(CYAN)$(BONUS_NAME) compiled!$(RESET)"

$(OBJ_DIR)/main.o: main.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "$(MAGENTA)Compiling: $< $(RESET)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "$(MAGENTA)Compiling: $< $(RESET)"

################################################################################
#                                  CLEAN / FCLEAN                              #
################################################################################

clean:
	@echo "\n$(RED)Removing: $(OBJ) $(OBJ_DIR) $(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Dependencies have been removed.$(RESET)"
	@echo "$(RED)Obj directory has been removed.$(RESET)\n"

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)

re: fclean all

bonus: $(BONUS_NAME)

################################################################################
#                                 DEPENDENCIES                                 #
################################################################################

-include $(DEPS)
-include $(BONUS_DEPS)

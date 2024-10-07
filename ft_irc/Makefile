################################################################################
#                                COMPILATION                                   #
################################################################################

NAME = ircserv
CC = c++
CFLAGS = -g3 -std=c++98 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/class
MAKEFLAGS += --no-print-directory

################################################################################
#                                  SOURCES                                     #
################################################################################

SRC_DIR = src
CLASS_DIR = $(SRC_DIR)/class
INCLUDE_DIR = ./includes

# fichier in src/class
SRC_CLASS = server.cpp client.cpp parse.cpp channel.cpp

# fichier in src/
SRC_FILES = handle_arg.cpp


SRC = $(addprefix $(CLASS_DIR)/, $(SRC_CLASS)) \
      $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
		main.cpp

################################################################################
#                                  OBJECTS / DEP                               #
################################################################################

OBJ_DIR = obj
OBJ := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS := $(OBJ:.o=.d)

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

$(OBJ_DIR)/main.o: main.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "$(MAGENTA)Compiling: $< $(DEF_COLOR)$(RESET)"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo "$(MAGENTA)Compiling: $< $(DEF_COLOR)$(RESET)"

################################################################################
#                                  CLEAN / FCLEAN                              #
################################################################################

clean:
	@echo "\n$(RED)Removing: $(OBJ) $(OBJ_DIR) $(DEF_COLOR)$(RESET)\n"
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Dependencies have been removed.$(RESET)"
	@echo "$(RED)Obj directory has been removed.$(RESET)\n"

fclean: clean
	@rm -f $(NAME)

re: fclean all

################################################################################
#                                 DEPENDENCIES                                 #
################################################################################

-include $(DEPS)
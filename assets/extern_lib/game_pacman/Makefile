PROJECT = lib_pacman

OBJ_DIR = ./obj

# General behavior

NAME_BIN = lib_arcade_pacman.so

SRC_GENDIR = ./
OBJ_GENDIR = $(OBJ_DIR)/

SRC_GEN = Pacman.cpp	\
	Player.cpp	\
	Ghost.cpp	\
	Square.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

ROOT_PROJECT = ./../../../

# Utility

INCDIRS = -I$(ROOT_PROJECT)includes/
LDFLAGS = -L$(ROOT_PROJECT)shared_lib/ -lComponents -lDirectoryReader
CC = g++ -g
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11
CFLAGS = $(FLAGS) $(INCDIRS)

RM		= rm -rf

# Rules

all: $(NAME_BIN)

install:
	cp $(NAME_BIN) $(ROOT_PROJECT)games/


$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -fPIC

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN) $(FLAGS) $(LDFLAGS) -shared

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)
	$(RM) $(ROOT_PROJECT)games/$(NAME_BIN)

re: fclean all

.PHONY: all clean fclean re

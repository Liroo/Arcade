PROJECT = lib_allegro

OBJ_DIR = ./obj

# General behavior

NAME_BIN = libGameTest.so

SRC_GENDIR = ./
OBJ_GENDIR = $(OBJ_DIR)/

SRC_GEN = LibGameTest.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

ROOT_PROJECT = ./../../../

# Utility

INCDIRS = -I$(ROOT_PROJECT)includes/
CC = g++
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
	$(CC) -o $@ -c $< $(CFLAGS) -fpic

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN) $(FLAGS) -shared

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)

re: fclean all

.PHONY: all clean fclean re

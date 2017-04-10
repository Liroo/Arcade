PROJECT = lib_directoryReader

OBJ_DIR = ./obj

# General behavior

NAME_BIN = libDirectoryReader.so

SRC_GENDIR = ./
OBJ_GENDIR = $(OBJ_DIR)/

SRC_GEN = DirectoryReader.cpp
INC_GEN = DirectoryReader.h

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

ROOT_PROJECT = ./../../../../

# Utility

CC = g++
LDFLAGS = -ldl
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11
CFLAGS = $(FLAGS)

RM		= rm -rf

# Rules

all: $(NAME_BIN)

install:
	cp $(NAME_BIN) $(ROOT_PROJECT)shared_lib/
	cp $(INC_GEN) $(ROOT_PROJECT)includes/


$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -fpic

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN) $(FLAGS) $(LDFLAGS) -shared

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)
	$(RM) $(ROOT_PROJECT)shared_lib/$(NAME_BIN)
	$(RM) $(ROOT_PROJECT)includes/$(INC_GEN)

re: fclean all

.PHONY: all clean fclean re

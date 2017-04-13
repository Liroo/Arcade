PROJECT = lib_caca

OBJ_DIR = ./obj

# General behavior

NAME_BIN = lib_arcade_caca.so

SRC_GENDIR = ./
OBJ_GENDIR = $(OBJ_DIR)/

SRC_GEN = Caca.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

ROOT_PROJECT = ./../../../

# Utility

INCDIRS = -I$(ROOT_PROJECT)includes/
CC = g++
LDFLAGS = $(shell pkg-config --libs --cflags caca)
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11
CFLAGS = $(FLAGS) $(INCDIRS)

RM		= rm -rf

# Rules

all: $(NAME_BIN)

debug: re
	$(CC) -o mainTest mainTest.cpp $(CFLAGS) \
		-Wl,-rpath,/mnt/hgfs/Epitech/CPP/Arcade/shared_lib/,-rpath,/mnt/hgfs/Epitech/CPP/Arcade/assets/extern_lib/lib_caca/ \
		-L./ lib_arcade_caca.so $(LDFLAGS)


install:
	cp $(NAME_BIN) $(ROOT_PROJECT)lib/


$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) -fpic

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN) $(FLAGS) $(LDFLAGS) -shared

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)
	$(RM) $(ROOT_PROJECT)lib/$(NAME_BIN)

re: fclean all

.PHONY: all clean fclean re

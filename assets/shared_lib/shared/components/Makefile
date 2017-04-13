PROJECT = lib_components

OBJ_DIR = ./obj

# General behavior

NAME_BIN = libComponents.so

SRC_GENDIR = ./
OBJ_GENDIR = $(OBJ_DIR)/

SRC_GEN = AnimatedObject.cpp \
	Button.cpp \
	Carroussel.cpp \
	Timer.cpp \
	Score.cpp

INC_GEN = AnimatedObject.h \
	Button.h \
	Carroussel.h \
	Timer.h \
	Score.h

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

ROOT_PROJECT = ./../../../../
INC_PATH = $(addprefix $(ROOT_PROJECT)includes/components/, $(INC_GEN))

# Utility

INCDIRS = -I$(ROOT_PROJECT)includes/
#LDFLAGS = -L$(ROOT_PROJECT)shared_lib/ -lDirectoryReader
CC = g++
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11
CFLAGS = $(FLAGS) $(INCDIRS)

RM		= rm -rf

# Rules

all: $(NAME_BIN)

install:
	cp $(NAME_BIN) $(ROOT_PROJECT)shared_lib/
	cp $(INC_GEN) $(ROOT_PROJECT)includes/components/


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
	$(RM) $(INC_PATH)

re: fclean all

.PHONY: all clean fclean re

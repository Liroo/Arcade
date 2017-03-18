PROJECT = cpp_arcade

OBJ_DIR = obj/

# General behavior of arcade

NAME_BIN = arcade

SRC_GENDIR = src/
OBJ_GENDIR = $(OBJ_DIR)src/

LIBLOADER_DIR = assets/LibLoader/

SRC_GEN = main.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

# Utility

INCDIRS := $(addprefix -I,$(shell find $(SRC_GENDIR) -type d -print)) -I./includes/
CC = g++
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11
CFLAGS = $(FLAGS) $(INCDIRS)

RM		= rm -rf

# Rules

all:
	@make --no-print-directory $(NAME_BIN)
	@make --no-print-directory -C $(LIBLOADER_DIR)

$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CC) -o $(NAME_BIN) $(OBJ_GEN)

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)

re: fclean all

.PHONY: all clean fclean re

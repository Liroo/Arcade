PROJECT = cpp_arcade

OBJ_DIR = obj/

# General behavior of arcade

NAME_BIN = arcade

SRC_GENDIR = src/
OBJ_GENDIR = $(OBJ_DIR)src/

SRC_GEN = main.cpp \
					core/Core.cpp

OBJ_GEN = $(addprefix $(OBJ_GENDIR), $(SRC_GEN:.cpp=.o))

# Utility

INCDIRS := $(addprefix -I,$(shell find $(SRC_GENDIR) -type d -print)) -I./includes/
CXX = g++
LDFLAGS = -ldl -L./shared_lib -lDirectoryReader
LDFLAGS += -Wl,-rpath=$(shell pwd)/shared_lib
FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++14
FLAGS += $(DEBUG)
CXXFLAGS = $(FLAGS) $(INCDIRS)

RM		= rm -rf

# Rules

all:
	@make --no-print-directory $(NAME_BIN)

# debug: FLAGS += -DDEBUG -g
debug: fclean
	@make --no-print-directory $(NAME_BIN) DEBUG='-DDEBUG -g'

$(OBJ_GENDIR)%.o: $(SRC_GENDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(NAME_BIN): $(NAME_LIB) $(OBJ_GEN)
	$(CXX) -o $(NAME_BIN) $(OBJ_GEN) $(FLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME_BIN)

re: fclean all

.PHONY: debug

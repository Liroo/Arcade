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

install:
	@make --no-print-directory -C "./assets/shared_lib/opaque/basic"
	@make --no-print-directory -C "./assets/shared_lib/opaque/basic" install
	@make --no-print-directory -C "./assets/shared_lib/shared/components"
	@make --no-print-directory -C "./assets/shared_lib/shared/components" install
	@make --no-print-directory -C "./assets/shared_lib/shared/lib_loader"
	@make --no-print-directory -C "./assets/shared_lib/shared/lib_loader" install
	@make --no-print-directory -C "./assets/extern_lib/game_menu"
	@make --no-print-directory -C "./assets/extern_lib/game_menu" install
	@make --no-print-directory -C "./assets/extern_lib/game_pacman"
	@make --no-print-directory -C "./assets/extern_lib/game_pacman" install
	@make --no-print-directory -C "./assets/extern_lib/game_snake"
	@make --no-print-directory -C "./assets/extern_lib/game_snake" install
	@make --no-print-directory -C "./assets/extern_lib/lib_allegro"
	@make --no-print-directory -C "./assets/extern_lib/lib_allegro" install
	@make --no-print-directory -C "./assets/extern_lib/lib_caca"
	@make --no-print-directory -C "./assets/extern_lib/lib_caca" install
	@make --no-print-directory -C "./assets/extern_lib/lib_libLapin"
	@make --no-print-directory -C "./assets/extern_lib/lib_libLapin" install
	@make --no-print-directory

reinstall:
	@make --no-print-directory -C "./assets/shared_lib/opaque/basic" re
	@make --no-print-directory -C "./assets/shared_lib/opaque/basic" install
	@make --no-print-directory -C "./assets/shared_lib/shared/components" re
	@make --no-print-directory -C "./assets/shared_lib/shared/components" install
	@make --no-print-directory -C "./assets/shared_lib/shared/lib_loader" re
	@make --no-print-directory -C "./assets/shared_lib/shared/lib_loader" install
	@make --no-print-directory -C "./assets/extern_lib/game_menu" re
	@make --no-print-directory -C "./assets/extern_lib/game_menu" install
	@make --no-print-directory -C "./assets/extern_lib/game_pacman" re
	@make --no-print-directory -C "./assets/extern_lib/game_pacman" install
	@make --no-print-directory -C "./assets/extern_lib/game_snake" re
	@make --no-print-directory -C "./assets/extern_lib/game_snake" install
	@make --no-print-directory -C "./assets/extern_lib/lib_allegro" re
	@make --no-print-directory -C "./assets/extern_lib/lib_allegro" install
	@make --no-print-directory -C "./assets/extern_lib/lib_caca" re
	@make --no-print-directory -C "./assets/extern_lib/lib_caca" install
	@make --no-print-directory -C "./assets/extern_lib/lib_libLapin" re
	@make --no-print-directory -C "./assets/extern_lib/lib_libLapin" install
	@make --no-print-directory re

uninstall:
	@make --no-print-directory -C "./assets/shared_lib/opaque/basic" fclean
	@make --no-print-directory -C "./assets/shared_lib/shared/components" fclean
	@make --no-print-directory -C "./assets/shared_lib/shared/lib_loader" fclean
	@make --no-print-directory -C "./assets/extern_lib/game_menu" fclean
	@make --no-print-directory -C "./assets/extern_lib/game_pacman" fclean
	@make --no-print-directory -C "./assets/extern_lib/game_snake" fclean
	@make --no-print-directory -C "./assets/extern_lib/lib_allegro" fclean
	@make --no-print-directory -C "./assets/extern_lib/lib_caca" fclean
	@make --no-print-directory -C "./assets/extern_lib/lib_libLapin" fclean
	@make --no-print-directory fclean

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

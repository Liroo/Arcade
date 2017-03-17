NAME_LIB = LibLoader.so
SRC_LIB = LibLoader.hpp

CXX = g++
RM = rm

FLAGS = -W -Werror -Wextra -Wall
FLAGS += -std=c++11

all:
	@make --no-print-directory $(NAME_LIB)

$(NAME_LIB):
	$(CXX) -shared -fpic -x c++ $(FLAGS) $(SRC_LIB) -o $(NAME_LIB)

clean:

fclean:
	$(RM) $(NAME_LIB)

re: fclean all

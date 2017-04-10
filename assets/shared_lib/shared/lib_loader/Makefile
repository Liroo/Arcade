PROJECT = lib_loader

# General behavior

NAME_INC = LibLoader.h

ROOT_PROJECT = ./../../../../

# Utility

RM		= rm -rf

# Rules

all:

install:
	cp $(NAME_INC) $(ROOT_PROJECT)includes/

clean:

fclean:	clean
	$(RM) $(NAME_BIN)
	$(RM) $(ROOT_PROJECT)includes/$(NAME_INC)

re: fclean all

.PHONY: all clean fclean re

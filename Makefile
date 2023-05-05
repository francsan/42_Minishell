#SETUP
NAME		=	minishell
CC			=	cc
FLAGS		=	-Wall -Wextra -Werror
FSAN		=	-g -fsanitize=address
RM			=	rm -rf

#FILES, DIRECTORIES AND PATHS
HEADER_F	=	minishell.h
HEADER_DIR	=	headers/
HEADER		=	$(addprefix $(HEADER_DIR), $(HEADER_F))
FUNCS_F		=	ft_calloc.c ft_split.c ft_strarr_cpy.c \
				ft_strarr_free.c ft_strdup.c ft_strjoin.c \
				ft_strlen.c ft_strncmp.c
FUNCS_DIR	=	functions/
FUNCS		=	$(addprefix $(FUNCS_DIR), $(FUNCS_F))
SOURCE_F	=	main.c parsing.c ft_minishell_split_utils.c \
				ft_minishell_split.c command_handling.c \
				pipes.c parsing_utils.c utils.c
SOURCE_DIR	=	sources/
SOURCE		=	$(addprefix $(SOURCE_DIR), $(SOURCE_F))

#OBJECTS
OBJS_S		=	$(SOURCE:.c=.o)
OBJS_F		=	$(FUNCS:.c=.o)

#COMMANDS
%.o: %.c
				@${CC} ${FLAGS} -c $< -o $@

$(NAME):		$(OBJS_S) $(OBJS_F)
				@$(CC) $(FLAGS) $(FSAN) $(OBJS_S) $(OBJS_F) -lreadline -o $(NAME)
				@echo "$(GREEN)$(NAME) created!$(DEFAULT)"

all:			$(NAME)

clean:
				@$(RM) $(OBJS_S) $(OBJS_F)
				@echo "$(YELLOW)object files deleted!$(DEFAULT)"

fclean:			clean
				@$(RM) $(NAME)
				@echo "$(RED)object files and executable deleted!$(DEFAULT)"

re:				fclean all

.PHONY:		all clean fclean re

#COLORS
RED		=	\033[1;31m
GREEN	=	\033[1;32m
YELLOW	=	\033[1;33m
DEFAULT	=	\033[0m
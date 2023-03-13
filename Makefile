# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avast <avast@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/10 11:41:55 by avast             #+#    #+#              #
#    Updated: 2023/03/13 11:30:08 by avast            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = src/errors.c \
	src/files.c \
	src/main.c \
	src/path.c \
	src/pid.c \
	src/pipes.c \

OBJ = $(SRC:.c=.o)

INCLUDES = includes/pipex.h \

CC = gcc

FLAGS = -Wall -Wextra -Werror -g3

LIBFT = ./libft/libft.a

LIBFTDIR = ./libft/

MAKEFLAGS += --no-print-directory

BLUE = \033[1;94m

PURPLE = \033[1;95m

GREEN = \033[1;92m

YELLOW = \033[1;93m

all: $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -I./includes -I./libft -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(INCLUDES)
	@$(CC) $(OBJ) -L$(LIBFTDIR) -lreadline -g3 -lft -o $(NAME)
	@printf "$(YELLOW)------Compilation executed------\n\n"

$(LIBFT):
	@make -C $(LIBFTDIR) 
	@printf "$(BLUE)--------Libft.a created----------\n\n"

clean:
	@/bin/rm -f $(OBJ)
	@make clean -C $(LIBFTDIR)
	@printf "$(PURPLE)------Object files deleted-------\n\n"

fclean: clean
	@/bin/rm -f $(NAME)
	@make fclean -C $(LIBFTDIR)
	@printf "$(GREEN)----Executable files deleted-----\n\n"

re: fclean all

.PHONY : all clean fclean re


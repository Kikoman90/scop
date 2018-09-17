# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/30 19:54:25 by fsidler           #+#    #+#              #
#    Updated: 2018/06/12 19:46:11 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = scop

CFLAGS = -Wall -Wextra -Werror -g

SDLFLAGS = $(shell sdl2-config --libs --cflags)

DIR_LIBFT = libs/libft

DIR_SRCS = sources

DIR_OBJS = tmp

HEADERS = includes

SRCS =	main.c \
		setup.c \
		parser.c \
		utils.c \
		clean_go.c \
		cleanup.c \
		display.c \
		error.c

OBJS = $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))

all: tmp $(NAME)

$(NAME): $(OBJS)
		@make -C $(DIR_LIBFT)
		$(CC) $(CFLAGS) -L $(DIR_LIBFT) -lft -o $@ $^ $(SDLFLAGS) -framework OpenGL

tmp:
		@mkdir -p tmp

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(HEADERS)/$(NAME).h
		$(CC) $(CFLAGS) -I $(HEADERS) -I/Users/fsidler/.brew/include/ -D_THREAD_SAFE -c $< -o $@

norme:
		@make norme -C $(DIR_LIBFT)
		@echo
		@norminette ./$(HEADERS)
		@echo
		@norminette ./$(DIR_SRCS)

clean:
		@rm -f $(OBJ)
		@make clean -C $(DIR_LIBFT)
		@rm -rf $(DIR_OBJS)

fclean: clean
		@rm -f $(NAME)
		@make fclean -C $(DIR_LIBFT)

re: fclean all

.PHONY: all, tmp, norme, clean, fclean, re

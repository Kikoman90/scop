# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/30 19:54:25 by fsidler           #+#    #+#              #
#    Updated: 2018/11/14 19:57:25 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = scop

CFLAGS = -Wall -Wextra -Werror -g

SDLFLAGS = $(shell sdl2-config --libs --cflags)

DIR_LIBFT = libs/libft

DIR_LIBMATH = libs/libmath

DIR_SRCS = sources

DIR_OBJS = tmp

HEADERS = includes

SRCS =	setup.c \
		shader.c \
		color.c \
		tga_loader.c \
		textures.c \
		parser.c \
		parser_utils.c \
		mtl_parse.c \
		attrib_parse.c \
		obj_parse.c \
		obj_get.c \
		node_create.c \
		node_add.c \
		node_clean.c \
		buffers_bind.c \
		uniforms_get.c \
		uniforms_set.c \
		events_handle.c \
		update.c \
		draw.c \
		cleanup.c \
		main.c \
		display.c \

OBJS = $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))

all: tmp $(NAME)

$(NAME): $(OBJS)
		@make -C $(DIR_LIBFT)
		@make -C $(DIR_LIBMATH)
		$(CC) $(CFLAGS) -L $(DIR_LIBFT) -lft -L $(DIR_LIBMATH) -lmath -o $@ $^ $(SDLFLAGS) -framework OpenGL

tmp:
		@mkdir -p tmp

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(HEADERS)/$(NAME).h
		$(CC) $(CFLAGS) -I $(HEADERS) -I/Users/fsidler/.brew/include/ -D_THREAD_SAFE -c $< -o $@

norme:
		@make norme -C $(DIR_LIBFT)
		@echo
		@make norme -C $(DIR_LIBMATH)
		@echo
		@norminette ./$(HEADERS)
		@echo
		@norminette ./$(DIR_SRCS)

clean:
		@rm -f $(OBJ)
		@make clean -C $(DIR_LIBFT)
		@make clean -C $(DIR_LIBMATH)
		@rm -rf $(DIR_OBJS)

fclean: clean
		@rm -f $(NAME)
		@make fclean -C $(DIR_LIBFT)
		@make fclean -C $(DIR_LIBMATH)

re: fclean all

.PHONY: all, tmp, norme, clean, fclean, re

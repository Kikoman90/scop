# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/12 17:25:31 by fsidler           #+#    #+#              #
#    Updated: 2018/06/12 17:25:32 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = libmath.a

CFLAGS = -Wall -Wextra -Werror

DIR_SRCS = sources

DIR_OBJS = tmp

HEADERS = includes

SRCS =	vector.c \
		vector_advop.c \
		vector_basop.c \
		matrix.c \

OBJS = $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))

all: tmp $(NAME)

$(NAME): $(OBJS)
		@ar rc $(NAME) $(OBJS)
		@ranlib $(NAME)

tmp:
		@mkdir -p tmp

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
		@$(CC) $(CFLAGS) -I $(HEADERS) -c $< -o $@

norme:
		norminette ./$(HEADERS)
		@echo
		norminette ./$(DIR_SRCS)

clean:		
		@rm -f $(OBJ)
		@rm -rf $(DIR_OBJS)

fclean:	clean	
		@rm -f $(NAME)

re: fclean all

.PHONY: all, tmp, norme, clean, fclean, re

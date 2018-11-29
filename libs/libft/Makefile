# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/02 11:09:55 by fsidler           #+#    #+#              #
#    Updated: 2018/11/29 14:39:08 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

DIR_SRCS = sources

DIR_OBJS = tmp

HEADERS = includes

SRCS = 	ft_bzero.c \
		ft_isblank.c \
  		ft_isdigit.c \
	  	ft_atoi.c \
		ft_atof_f.c \
		ft_clamp.c \
	  	ft_strcmp.c \
	  	ft_strcat.c \
	  	ft_strchr.c \
	  	ft_strrchr.c \
	  	ft_strcpy.c \
	  	ft_strdup.c \
	  	ft_strlen.c \
		ft_putchar.c \
		ft_putstr.c \
	  	ft_putendl.c \
	  	ft_strnew.c \
	  	ft_strsub.c \
	  	ft_strjoin.c \
		ft_strjoin_bf.c \
		ft_strjoin_lf.c \
		ft_strjoin_rf.c \
		ft_strword.c \
	  	ft_itoa.c \
		ft_wordoffset.c \
		ft_error.c \
		ft_file.c \
		ft_get_files.c \

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

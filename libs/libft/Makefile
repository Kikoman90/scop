# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsidler <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/02 11:09:55 by fsidler           #+#    #+#              #
#    Updated: 2015/12/21 18:44:29 by fsidler          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

DIR_SRCS = sources

DIR_OBJS = tmp

HEADERS = includes

SRCS = 	ft_isascii.c \
  		ft_isprint.c \
  		ft_isalpha.c \
  		ft_isdigit.c \
	  	ft_isalnum.c \
	  	ft_tolower.c \
	  	ft_toupper.c \
	  	ft_atoi.c \
		ft_atof.c \
	 	ft_bzero.c \
	  	ft_memccpy.c \
	  	ft_memcpy.c \
	  	ft_memchr.c \
	  	ft_memmove.c \
	  	ft_memset.c \
	  	ft_memcmp.c \
	  	ft_strcmp.c \
	  	ft_strncmp.c \
	  	ft_strcat.c \
	  	ft_strncat.c \
	  	ft_strchr.c \
	  	ft_strrchr.c \
	  	ft_strcpy.c \
	  	ft_strncpy.c \
	  	ft_strdup.c \
	  	ft_strstr.c \
	  	ft_strnstr.c \
	  	ft_strlen.c \
	  	ft_strlcat.c \
	  	ft_memalloc.c \
	  	ft_memdel.c \
	  	ft_strdel.c \
	  	ft_strclr.c \
	  	ft_striter.c \
	  	ft_striteri.c \
	  	ft_putchar.c \
	  	ft_putstr.c \
	  	ft_putendl.c \
	  	ft_putnbr.c \
	  	ft_putchar_fd.c \
	  	ft_putstr_fd.c \
	  	ft_putendl_fd.c \
	  	ft_putnbr_fd.c \
	  	ft_strnew.c \
	  	ft_strmap.c \
	  	ft_strmapi.c \
	  	ft_strsub.c \
	  	ft_strjoin.c \
	  	ft_strtrim.c \
	  	ft_itoa.c \
	  	ft_strsplit.c \
	  	ft_strequ.c \
	  	ft_strnequ.c \
	  	ft_abs.c \
	  	ft_lstdelone.c \
	  	ft_lstdel.c \
	  	ft_lstadd.c \
	  	ft_lstiter.c \
	  	ft_lstnew.c \
	  	ft_lstmap.c \
	  	ft_isspace.c \
	  	ft_fdim.c \
	  	ft_round.c \
	  	ft_copysign.c \
	  	ft_signbit.c \

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
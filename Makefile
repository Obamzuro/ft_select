# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/23 14:59:44 by obamzuro          #+#    #+#              #
#    Updated: 2018/05/28 15:24:08 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

SRCNAME = main.c

HDR = include/ft_select.h

SRC = $(addprefix src/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	make -C ftprintf
	gcc -g $(OBJ) -I include -L libft -lft -L ftprintf -lftprintf -ltermcap -o 1

%.o: %.c $(HDR)
	gcc -g -c $< -o $@ -I include

clean:
	make -C libft clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pok]" -exec rm -rf {} \;

fclean: clean
	make -C libft clean
	make -C ftprintf clean
	rm -rf $(NAME)

re: fclean all

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/23 14:59:44 by obamzuro          #+#    #+#              #
#    Updated: 2018/05/30 13:11:20 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

SRCNAME = main.c		\
		  sel_putchar.c	\

HDR = include/ft_select.h

SRC = $(addprefix src/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libf
	make -C ftprintf
	gcc -g $(OBJ) -I include -L libf -lft -L ftprintf -lftprintf -ltermcap -o asd

%.o: %.c $(HDR)
	gcc -g -c $< -o $@ -I include

clean:
	make -C libf clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pok]" -exec rm -rf {} \;

fclean: clean
	make -C libf clean
	make -C ftprintf clean
	rm -rf $(NAME)

re: fclean all

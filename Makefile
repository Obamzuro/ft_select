# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/23 14:59:44 by obamzuro          #+#    #+#              #
#    Updated: 2018/05/28 13:21:10 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

SRCNAME = main.c

HDR = includes/ft_select.h

SRC = $(addprefix src/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	make -C ftprintf
	gcc $(OBJ) -I includes -L libft -lft -L ftprintf -lftprintf

%.o: %.c $(HDR)
	gcc -c $< -o $@ -I includes

clean:
	make -C libft clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pok]" -exec rm -rf {} \;

fclean: clean
	make -C libft clean
	make -C ftprintf clean
	rm -rf $(NAME)

re: fclean all

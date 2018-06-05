# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/23 14:59:44 by obamzuro          #+#    #+#              #
#    Updated: 2018/06/05 12:39:29 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

SRCNAME = main.c		\
		  sel_putchar.c	\
		  arrow_handling.c\
		  button_handling.c\
		  getsize_term.c\
		  init_term.c\
		  print_files.c\
		  print_style.c\
		  signal_handling.c

HDR = include/ft_select.h

SRC = $(addprefix src/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	make -C libf
	make -C ftprintf
	gcc $(FLAGS) $(OBJ) -I include -L libf -lft -L ftprintf -lftprintf -ltermcap -o $(NAME)

%.o: %.c $(HDR)
	gcc $(FLAGS) -c $< -o $@ -I include

clean:
	make -C libf clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pok]" -exec rm -rf {} \;
	rm -rf $(OBJ)

fclean: clean
	make -C libf fclean
	make -C ftprintf fclean
	rm -rf $(NAME)

re: fclean all

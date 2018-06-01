/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 15:07:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/01 13:49:46 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_SELECT_H
# define __FT_SELECT_H
# include "ft_printf.h"
# include "libft.h"
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <stdio.h>
# include <termcap.h>
# include <signal.h>
# include <sys/ioctl.h>
# define ESC "\x1B"
# define DOWN "\x1B[B"
# define UP "\x1B[A"
# define LEFT "\x1B[D"
# define RIGHT "\x1B[C"
# define SPACE " "
# define SUSPEND "\x1A"
# define DEL "\x1B[3~"
# define DEFAULT "\x1B[0m"
# define BACKSPACE "\x7F"
# define ENTER "\x0A"

typedef struct	s_file_list
{
	char				*name;
	int					ispressed;
	struct s_file_list	*prev;
	struct s_file_list	*next;
}				t_file_list;

int				sel_putchar(int c);
void			quit_handler(int sig);
void			fill_signal_handlers(void);
void			handle_exit(int sig);
#endif

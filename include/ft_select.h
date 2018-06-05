/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 15:07:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:27:27 by obamzuro         ###   ########.fr       */
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
# include <sys/stat.h>
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

# define ANSI_COLOR_BLACK    "\x1b[30m"
# define ANSI_COLOR_RED      "\x1b[31m"
# define ANSI_COLOR_GREEN    "\x1b[32m"
# define ANSI_COLOR_YELLOW   "\x1b[33m"
# define ANSI_COLOR_BLUE     "\x1b[34m"
# define ANSI_COLOR_MAGENTA  "\x1b[35m"
# define ANSI_COLOR_CYAN     "\x1b[36m"
# define ANSI_COLOR_WHITE    "\x1b[36m"

# define ANSI_COLOR_BLACKN   "\e[30m"
# define ANSI_COLOR_REDN     "\e[31m"
# define ANSI_COLOR_GREENN   "\e[32m"
# define ANSI_COLOR_YELLOWN  "\e[33m"
# define ANSI_COLOR_BLUEN    "\e[34m"
# define ANSI_COLOR_MAGENTAN "\e[35m"
# define ANSI_COLOR_CYANN    "\e[36m"
# define ANSI_COLOR_WHITEN   "\e[36m"

# define ANSI_COLOR_BBLACK   "\x1b[40m"
# define ANSI_COLOR_BRED     "\x1b[41m"
# define ANSI_COLOR_BGREEN   "\x1b[42m"
# define ANSI_COLOR_BYELLOW  "\x1b[43m"
# define ANSI_COLOR_BBLUE    "\x1b[44m"
# define ANSI_COLOR_BMAGENTA "\x1b[45m"
# define ANSI_COLOR_BCYAN    "\x1b[46m"
# define ANSI_COLOR_BWHITE   "\x1b[46m"
# define ANSI_COLOR_RESET    "\x1b[0m"

# define ANSI_COLOR_BBLACKN   "\e[40m"
# define ANSI_COLOR_BREDN     "\e[41m"
# define ANSI_COLOR_BGREENN   "\e[42m"
# define ANSI_COLOR_BYELLOWN  "\e[43m"
# define ANSI_COLOR_BBLUEN    "\e[44m"
# define ANSI_COLOR_BMAGENTAN "\e[45m"
# define ANSI_COLOR_BCYANN    "\e[46m"
# define ANSI_COLOR_BWHITEN   "\e[46m"

typedef struct	s_file_list
{
	char				*name;
	int					ispressed;
	int					rownum;
	int					colnum;
	int					row;
	int					col;
	struct s_file_list	*prev;
	struct s_file_list	*next;
}				t_file_list;

int				sel_putchar(int c);
void			quit_handler(int sig);
void			handle_exit();

int				press_del(t_file_list **listbeg,
		t_file_list **listsel);
void			press_space(t_file_list *listbeg,
		t_file_list **listsel, int *flag);
void			press_return(t_file_list *listbeg);
void			press_right(t_file_list *listbeg,
		t_file_list **listsel, int *flag);
void			press_left(t_file_list *listbeg,
		t_file_list **listsel, int *flag);
void			press_up(t_file_list *listbeg,
		t_file_list **listsel, int *flag);
void			press_down(t_file_list *listbeg,
		t_file_list **listsel, int *flag);

void			term_associate(void);
void			set_noncanon(void);
void			fill_list(char **argv, t_file_list **listbeg);
void			fill_signal_handlers(void);

void			print_files(t_file_list *plistbeg,
		t_file_list *plistsel, int mode, t_file_list *listiter);
void			print_files_padding(t_file_list *listiter,
		int maxwordlen);
void			print_files_style(t_file_list *listiter,
		t_file_list *listsel);

int				find_longest_word(t_file_list *listbeg,
		int *amfiles);
int				get_tty_size(int amfiles, int *cols,
		int *rows, int maxwordlen);
#endif

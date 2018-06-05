/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:20:55 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:20:58 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	handle_exit(void)
{
	struct termios		tty;

	tcgetattr(2, &tty);
	tty.c_lflag |= ICANON | ECHO;
	tcsetattr(2, TCSANOW, &tty);
	ft_fprintf(2, tgetstr("te", 0));
	ft_fprintf(2, tgetstr("ve", 0));
}

void	quit_handler(int sig)
{
	handle_exit();
	if (sig != SIGTSTP)
		exit(0);
	else
	{
		signal(SIGTSTP, SIG_DFL);
		ioctl(0, TIOCSTI, SUSPEND);
	}
}

void	preparation(int sig)
{
	sig = 0;
	set_noncanon();
	fill_signal_handlers();
	print_files(0, 0, 0, 0);
}

void	winchange(int sig)
{
	sig = 0;
	print_files(0, 0, 0, 0);
}

void	fill_signal_handlers(void)
{
	signal(SIGINT, quit_handler);
	signal(SIGQUIT, quit_handler);
	signal(SIGTSTP, quit_handler);
	signal(SIGCONT, preparation);
	signal(SIGWINCH, winchange);
	signal(SIGABRT, quit_handler);
	signal(SIGTERM, quit_handler);
}

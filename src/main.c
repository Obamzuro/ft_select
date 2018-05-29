/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/29 22:18:25 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	term_associate(void)
{
	char	buf[2048];
	char	*termtype;
	int		success;

	termtype = getenv("TERM");
	if (!termtype)
	{
		ft_fprintf(2, "Couldn't find an env TERM\n");
		exit(EXIT_FAILURE);
	}
	success = tgetent(buf, termtype);
	if (success < 0)
	{
		ft_fprintf(2, "Couldn't access the termcap data base\n");
		exit(EXIT_FAILURE);
	}
	if (!success)
	{
		ft_fprintf(2, "Terminal type '%s' is not defined.\n", termtype);
		exit(EXIT_FAILURE);
	}
}

void	set_noncanon(void)
{
	struct termios		savetty;
	struct termios		tty;

	if (!isatty(0))
	{
		ft_fprintf(2, "stdin not terminal\n");
		exit(EXIT_FAILURE);
	}

	tcgetattr(0, &tty);
	savetty = tty;
	tty.c_lflag &= ~(ICANON);
	tty.c_cc[VMIN] = 1;
	tcsetattr(0, TCSAFLUSH, &tty);
	ft_printf("%c[2J", 27);
}

static int		sel_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

void			cycle(void)
{
	char ch;

	while (1)
	{
		read(0, &ch, 1);
		if (ch == 'q')
			break ;
		if (ch == '\t')
			break ;
		if (ch == 'w')
			tputs("qwe", 1, sel_putchar);
	}

}

int		main(int argc, char **argv)
{
	term_associate();
	set_noncanon();
	cycle();
}

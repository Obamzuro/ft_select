/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/30 14:17:30 by obamzuro         ###   ########.fr       */
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

struct termios	*set_noncanon(void)
{
	struct termios		*savetty;
	struct termios		tty;

	if (!isatty(0))
	{
		ft_fprintf(2, "stdin not terminal\n");
		exit(EXIT_FAILURE);
	}

	savetty = (struct termios *)malloc(sizeof(struct termios));
	tcgetattr(0, &tty);
	*savetty = tty;
	tty.c_lflag &= ~(ICANON|ECHO);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tty);
	char *buffer;

	buffer = (char *)malloc(2048);
	tputs(tgetstr("ei", &buffer), 1, sel_putchar);
//	tputs(tgetstr("ve", &buffer), 1, sel_putchar);
	return (savetty);
}

void	print_args(char **argv)
{
	int		i;

	i = 0;
	while (argv[++i])
		ft_printf("%s\n", argv[i]);
}

void			cycle()
{
	char *buffer;

	buffer = (char *)malloc(2048);
	char buf[8];
	char *a;
//	tputs(tgetstr("vi", &buffer), 1, sel_putchar);
	a = tgoto(tgetstr("up", &buffer), 0, 0);
//	tputs(a, 2, sel_putchar);
	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		//ft_printf("\033[2J");
//		ft_printf("%.2hhx %.2hhx %.2hhx %.2hhx %.2hhx %.2hhx %.2hhx %.2hhx \n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
//		ft_printf(" %c  %c  %c  %c  %c  %c  %c  %c \n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		//fflush(stdout);
		if (!ft_strcmp(buf, "q"))
			break ;
		else if (!ft_strcmp(a, buf))
			ft_printf("ggag");
//			break ;
//		if (ch == '\t')
//			break ;
//		if (ch == 'w')
//			tputs("qwe", 1, sel_putchar);
	}
}

int		main(int argc, char **argv)
{
	struct termios		*savetty;
	char *buf;

	buf = (char *)malloc(2048);
	term_associate();
	savetty = set_noncanon();
	tputs(tgetstr("cl", &buf), 1, sel_putchar);
	print_args(argv);
	cycle();
	tcsetattr(0, TCSANOW, savetty);
	free(savetty);
	system("leaks asd");
}

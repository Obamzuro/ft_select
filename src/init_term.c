/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:22:31 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:22:37 by obamzuro         ###   ########.fr       */
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
	struct termios		tty;

	if (!isatty(0))
	{
		ft_fprintf(2, "stdin not terminal\n");
		exit(EXIT_FAILURE);
	}
	tcgetattr(2, &tty);
	tty.c_lflag &= ~(ICANON | ECHO | CREAD);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(2, TCSANOW, &tty);
	ft_fprintf(2, tgetstr("ti", 0));
	ft_fprintf(2, tgetstr("vi", 0));
}

void	fill_list(char **argv, t_file_list **listbeg)
{
	int				i;
	t_file_list		*listiter;

	if (argv[1])
	{
		*listbeg = malloc(sizeof(t_file_list));
		ft_bzero(*listbeg, sizeof(t_file_list));
		(*listbeg)->name = argv[1];
	}
	listiter = *listbeg;
	i = 1;
	while (argv[++i])
	{
		listiter->next = malloc(sizeof(t_file_list));
		ft_bzero(listiter->next, sizeof(t_file_list));
		listiter->next->name = argv[i];
		listiter->next->prev = listiter;
		listiter = listiter->next;
	}
	if (listiter)
	{
		listiter->next = *listbeg;
		listiter->next->prev = listiter;
	}
}

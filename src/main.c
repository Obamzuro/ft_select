/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/31 21:30:16 by obamzuro         ###   ########.fr       */
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

void		set_noncanon(void)
{
	struct termios		tty;

	if (!isatty(0))
	{
		ft_fprintf(2, "stdin not terminal\n");
		exit(EXIT_FAILURE);
	}

	tcgetattr(0, &tty);
	tty.c_lflag &= ~(ICANON|ECHO);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tty);
	tputs(tgetstr("ti", 0), 1, sel_putchar);
	tputs(tgetstr("vi", 0), 1, sel_putchar);
//	tputs(tgetstr("ve", &buffer), 1, sel_putchar);
}

void	print_files(t_file_list *plistbeg,
		t_file_list *plistsel)
{
	static t_file_list	*listbeg;
	static t_file_list	*listsel;
	t_file_list			*listiter;
	int					count;

	if (plistbeg)
		listbeg = plistbeg;
	if (plistsel)
		listsel = plistsel;
	if (!listbeg || !listsel)
		return ;
	tputs(tgetstr("cl", 0), 1, sel_putchar);
	listiter = listbeg;
	count = 1;
	while (listiter != listbeg || count)
	{
		if (listiter->ispressed)
			ft_printf("%s", tgetstr("mr", 0));
		if (listiter == listsel)
			ft_printf("%s%s%s\n", tgetstr("us", 0),
					listiter->name, tgetstr("ue", 0));
		else
			ft_printf("%s\n", listiter->name);
		listiter = listiter->next;
		ft_printf(DEFAULT);
		count = 0;
	}
}

void	fill_list(char **argv, t_file_list **listbeg)
{
	int				i;
	t_file_list		*listiter;

	if (argv[1])
	{
		*listbeg = malloc(sizeof(t_file_list));
		(*listbeg)->name = argv[1];
		(*listbeg)->ispressed = 0;
	}
	listiter = *listbeg;
	i = 1;
	while (argv[++i])
	{
		listiter->next = malloc(sizeof(t_file_list));
		listiter->next->name = argv[i];
		listiter->next->prev = listiter;
		listiter->next->ispressed = 0;
		listiter = listiter->next;
	}
	if (listiter)
	{
		listiter->next = *listbeg;
		listiter->next->prev = listiter;
	}
}

void			press_up(t_file_list *listbeg,
		t_file_list **listsel)
{
	*listsel = (*listsel)->next;
	print_files(listbeg, *listsel);
}

void			press_down(t_file_list *listbeg,
		t_file_list **listsel)
{
	*listsel = (*listsel)->prev;
	print_files(listbeg, *listsel);
}

void			press_del(t_file_list **listbeg,
		t_file_list **listsel)
{
	t_file_list		*listnext;

	if ((*listbeg)->next == *listbeg)
		exit(EXIT_SUCCESS);
	if (*listsel == *listbeg)
		*listbeg = (*listbeg)->next;
	listnext = (*listsel)->next;
	(*listsel)->next->prev = (*listsel)->prev;
	(*listsel)->prev->next = (*listsel)->next;
	free(*listsel);
	*listsel = listnext;
	print_files(*listbeg, *listsel);
}

void			press_space(t_file_list *listbeg,
		t_file_list **listsel)
{
	(*listsel)->ispressed = !(*listsel)->ispressed;
	(*listsel) = (*listsel)->next;
	print_files(listbeg, *listsel);
}

void	fill_signal_handlers();
void	handle_exit(int sig);

void			press_return(t_file_list *listbeg)
{
	int			count;
	int			firstword;
	t_file_list	*listiter;

	handle_exit(0);
	listiter = listbeg;
	count = 1;
	firstword = 1;
	while (listiter != listbeg || count)
	{
		if (listiter->ispressed)
		{
			if (!firstword)
				ft_printf(" ");
			ft_printf("%s", listiter->name);
			firstword = 0;
		}
		listiter = listiter->next;
		count = 0;
	}
	exit(0);
}

void			cycle(t_file_list **listbeg,
		t_file_list **listsel)
{
	char	buf[8];
	char	*a;

	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		if (!ft_strcmp(buf, ESC))
			press_return(*listbeg);
		else if (!ft_strcmp(DOWN, buf))
			press_down(*listbeg, listsel);
		else if (!ft_strcmp(UP, buf))
			press_up(*listbeg, listsel);
		else if (!ft_strcmp(SPACE, buf))
			press_space(*listbeg, listsel);
		else if (!ft_strcmp(DEL, buf))
			press_del(listbeg, listsel);
	}
}

void	handle_exit(int sig)
{
	struct termios		tty;

	tcgetattr(0, &tty);
	tty.c_lflag |= ICANON|ECHO;
	tcsetattr(0, TCSANOW, &tty);
	tputs(tgetstr("te", 0), 1, sel_putchar);
	tputs(tgetstr("ve", 0), 1, sel_putchar);
}

void	quit_handler(int sig)
{
	handle_exit(sig);
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
	set_noncanon();
	fill_signal_handlers();
	print_files(0, 0);
}

void	fill_signal_handlers()
{
	signal(SIGINT, quit_handler);
	signal(SIGQUIT, quit_handler);
	signal(SIGTSTP, quit_handler);
	signal(SIGCONT, preparation);
}

int		main(int argc, char **argv)
{
	t_file_list			*listsel;
	t_file_list			*listbeg;
	char *a;

	listsel = 0;
	term_associate();
	fill_list(argv, &listbeg);
	listsel = listbeg;

	set_noncanon();
	fill_signal_handlers();
	print_files(listbeg, listsel);

	cycle(&listbeg, &listsel);
	handle_exit(0);
	system("leaks asd");
}

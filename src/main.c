/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/01 14:08:47 by obamzuro         ###   ########.fr       */
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

int		find_longest_word(t_file_list *listbeg,
		int *amfiles)
{
	int					flag;
	static t_file_list	*listiter;
	int					max;
	int					cur;

	flag = 1;
	listiter = listbeg;
	max = 0;
	*amfiles = 0;
	while (listiter != listbeg || flag)
	{
		cur = ft_strlen(listiter->name);
		if (cur > max)
			max = cur;
		listiter = listiter->next;
		flag = 0;
		++*amfiles;
	}
	return (max);
}

void	print_files_padding(t_file_list *listiter,
		int maxwordlen)
{
	int i;
	int offset;

	i = -1;
	offset = maxwordlen + 1 - ft_strlen(listiter->name);
	while (++i < offset)
		write(1, " ", 1);
}

int		get_tty_size(int amfiles, int *cols,
		int *rows, int maxwordlen)
{
	struct winsize	ws;

	*rows = 0;
	*cols = 0;
	if (ioctl(0, TIOCGWINSZ, &ws) != 0)
	{
		ft_printf("IOCTL ERROR\n");
		return (1);
	}
	*cols = ws.ws_col / (maxwordlen + 1);
	*rows = amfiles / *cols;
	if (ws.ws_col < *cols || ws.ws_row < *rows)
	{
		ft_printf("NO SPACE\n");
		return (1);
	}
	return (0);
}

void	print_files(t_file_list *plistbeg,
		t_file_list *plistsel)
{
	static t_file_list	*listbeg;
	static t_file_list	*listsel;
	t_file_list			*listiter;
	int					count;
	int					maxwordlen;
	int					cols;
	int					rows;
	int					amfiles;

	if (plistbeg)
		listbeg = plistbeg;
	if (plistsel)
		listsel = plistsel;
	if (!listbeg || !listsel)
		return ;
	maxwordlen = find_longest_word(listbeg, &amfiles);
	tputs(tgetstr("cl", 0), 1, sel_putchar);
	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	listiter = listbeg;
	count = 1;
	while (listiter != listbeg || count == 1)
	{
		if (listiter->ispressed)
			ft_printf("%s", tgetstr("mr", 0));
		if (listiter == listsel)
			ft_printf("%s%s%s", tgetstr("us", 0),
					listiter->name, tgetstr("ue", 0));
		else
			ft_printf("%s", listiter->name);
		ft_printf(DEFAULT);
		print_files_padding(listiter, maxwordlen);
		if (count % cols == 0)
			ft_printf("\n");
		listiter = listiter->next;
		++count;
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

void			press_right(t_file_list *listbeg,
		t_file_list **listsel)
{
	*listsel = (*listsel)->next;
	print_files(listbeg, *listsel);
}

void			press_left(t_file_list *listbeg,
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

void			press_up(t_file_list *listbeg,
		t_file_list **listsel)
{
	int maxwordlen;
	int amfiles;
	int rows;
	int cols;
	int i;

	maxwordlen = find_longest_word(listbeg, &amfiles);
	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	i = -1;
	while (++i < cols)
		*listsel = (*listsel)->prev;
	print_files(listbeg, *listsel);
}

void			press_down(t_file_list *listbeg,
		t_file_list **listsel)
{
	int maxwordlen;
	int amfiles;
	int rows;
	int cols;
	int i;

	maxwordlen = find_longest_word(listbeg, &amfiles);
	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	i = -1;
	while (++i < cols)
		*listsel = (*listsel)->next;
	print_files(listbeg, *listsel);
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
		if (!ft_strcmp(ESC, buf))
			quit_handler(0);
		else if (!ft_strcmp(ENTER, buf))
			press_return(*listbeg);
		else if (!ft_strcmp(UP, buf) || !ft_strcmp("k", buf))
			press_up(*listbeg, listsel);
		else if (!ft_strcmp(DOWN, buf) || !ft_strcmp("j", buf))
			press_down(*listbeg, listsel);
		else if (!ft_strcmp(LEFT, buf) || !ft_strcmp("h", buf))
			press_left(*listbeg, listsel);
		else if (!ft_strcmp(RIGHT, buf) || !ft_strcmp("l", buf))
			press_right(*listbeg, listsel);
		else if (!ft_strcmp(SPACE, buf))
			press_space(*listbeg, listsel);
		else if (!ft_strcmp(DEL, buf) || !ft_strcmp(BACKSPACE, buf))
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

void	winchange(int sig)
{
	print_files(0, 0);
}

void	fill_signal_handlers()
{
	signal(SIGINT, quit_handler);
	signal(SIGQUIT, quit_handler);
	signal(SIGTSTP, quit_handler);
	signal(SIGCONT, preparation);
	signal(SIGWINCH, winchange);
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

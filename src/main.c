/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/03 19:23:15 by obamzuro         ###   ########.fr       */
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
	tcgetattr(2, &tty);
	tty.c_lflag &= ~(ICANON | ECHO);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(2, TCSANOW, &tty);
	ft_fprintf(2, tgetstr("ti", 0));
	ft_fprintf(2, tgetstr("vi", 0));
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
		write(2, " ", 1);
}

int		get_tty_size(int amfiles, int *cols,
		int *rows, int maxwordlen)
{
	struct winsize	ws;

	*rows = 0;
	*cols = 0;
	if (ioctl(0, TIOCGWINSZ, &ws) != 0)
	{
		ft_fprintf(2, "IOCTL ERROR\n");
		return (1);
	}
	*cols = ws.ws_col / (maxwordlen + 1);
	if (!*cols)
	{
		ft_fprintf(2, "NO SPACE\n");
		return (1);
	}
	*rows = amfiles / *cols;
	if (ws.ws_col < *cols || ws.ws_row < *rows)
	{
		ft_fprintf(2, "NO SPACE\n");
		return (1);
	}
	return (0);
}

static void		print_color_name_dir(struct stat *tempstat)
{
	if (tempstat->st_mode & 02 && tempstat->st_mode & 01000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BGREEN);
	else if (tempstat->st_mode & 02 && !(tempstat->st_mode & 01000))
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BYELLOWN);
	else
		ft_fprintf(2, ANSI_COLOR_BLUE);
}

static void		print_color_name_exec(struct stat *tempstat)
{
	if (tempstat->st_mode & 04000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BRED);
	else if (tempstat->st_mode & 02000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BCYAN);
	else
		ft_fprintf(2, ANSI_COLOR_RED);
}

void			print_color_name(struct stat *tempstat)
{
	if (S_ISDIR(tempstat->st_mode))
		print_color_name_dir(tempstat);
	else if (S_ISLNK(tempstat->st_mode))
		ft_fprintf(2, ANSI_COLOR_MAGENTA);
	else if (S_ISSOCK(tempstat->st_mode))
		ft_fprintf(2, ANSI_COLOR_GREENN);
	else if (S_ISFIFO(tempstat->st_mode))
		ft_fprintf(2, ANSI_COLOR_YELLOWN);
	else if (S_ISBLK(tempstat->st_mode))
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLUE, ANSI_COLOR_BCYAN);
	else if (S_ISCHR(tempstat->st_mode))
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLUE, ANSI_COLOR_BYELLOWN);
	else if (tempstat->st_mode & 0100)
		print_color_name_exec(tempstat);
}

void	print_files_style(t_file_list *listiter,
		t_file_list *listsel)
{
	struct stat		tempstat;

	if (listiter->ispressed)
		ft_fprintf(2, tgetstr("mr", 0));
	if (listiter == listsel)
		ft_fprintf(2, tgetstr("us", 0));
	if (lstat(listiter->name, &tempstat) == -1)
		ft_fprintf(2, ANSI_COLOR_YELLOW);
	print_color_name(&tempstat);
}

void	get_cursor_position2(t_file_list *listiter)
{
	char	temp;

	read(2, &temp, 1);
	while (temp >= '0' && temp <= '9')
	{
		listiter->col = listiter->col * 10 + (temp - '0');
		read(2, &temp, 1);
	}
	if (temp != 'R')
		return ;
}

void	get_cursor_position(t_file_list *listiter)
{
	char	temp;

	listiter->row = 0;
	listiter->col = 0;
	write(2, "\x1B[6n", 4);
	read(2, &temp, 1);
	if (temp != '\x1b')
		return ;
	read(2, &temp, 1);
	if (temp != '[')
		return ;
	read(2, &temp, 1);
	while (temp >= '0' && temp <= '9')
	{
		listiter->row = listiter->row * 10 + (temp - '0');
		read(2, &temp, 1);
	}
	if (temp != ';')
		return ;
	get_cursor_position2(listiter);
}

void	print_special_file(t_file_list *listiter,
		t_file_list *listsel)
{
	ft_fprintf(2, tgoto(tgetstr("cm", 0), listiter->col - 1,
				listiter->row - 1));
	print_files_style(listiter, listsel);
	ft_fprintf(2, listiter->name);
	ft_fprintf(2, DEFAULT);
}

void	print_files_inner(t_file_list *listbeg,
		t_file_list *listsel, int maxwordlen, int amfiles)
{
	t_file_list		*listiter;
	int				cols;
	int				rows;
	int				count;

	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	listiter = listbeg;
	count = 1;
	while (listiter != listbeg || count == 1)
	{
		get_cursor_position(listiter);
		print_files_style(listiter, listsel);
		ft_fprintf(2, listiter->name);
		ft_fprintf(2, DEFAULT);
		print_files_padding(listiter, maxwordlen);
		if (count % cols == 0)
			ft_fprintf(2, "\n");
		listiter = listiter->next;
		++count;
	}
}

void	print_files(t_file_list *plistbeg,
		t_file_list *plistsel, int mode, t_file_list *listiter)
{
	static t_file_list	*listbeg;
	static t_file_list	*listsel;
	int					maxwordlen;
	int					amfiles;

	if (plistbeg)
		listbeg = plistbeg;
	if (plistsel)
		listsel = plistsel;
	if (!listbeg || !listsel)
		return ;
	if (mode)
	{
		print_special_file(listiter, listsel);
		return ;
	}
	maxwordlen = find_longest_word(listbeg, &amfiles);
	ft_fprintf(2, tgetstr("cl", 0));
	print_files_inner(listbeg, listsel, maxwordlen, amfiles);
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

void			press_right(t_file_list *listbeg,
		t_file_list **listsel)
{
	*listsel = (*listsel)->next;
	print_files(0, *listsel, 1, (*listsel)->prev);
	print_files(0, *listsel, 1, *listsel);
}

void			press_left(t_file_list *listbeg,
		t_file_list **listsel)
{
	*listsel = (*listsel)->prev;
	print_files(0, *listsel, 1, (*listsel)->next);
	print_files(0, *listsel, 1, *listsel);
}

void			press_del(t_file_list **listbeg,
		t_file_list **listsel)
{
	t_file_list		*listnext;
	int			count;

	if ((*listbeg)->next == *listbeg)
		exit(EXIT_SUCCESS);
	if (*listsel == *listbeg)
		*listbeg = (*listbeg)->next;
	listnext = (*listsel)->next;
	(*listsel)->next->prev = (*listsel)->prev;
	(*listsel)->prev->next = (*listsel)->next;
	free(*listsel);
	*listsel = listnext;
	print_files(*listbeg, *listsel, 0, 0);
}

void			press_space(t_file_list *listbeg,
		t_file_list **listsel)
{
	(*listsel)->ispressed = !(*listsel)->ispressed;
	(*listsel) = (*listsel)->next;
	print_files(0, *listsel, 1, (*listsel)->prev);
	print_files(0, *listsel, 1, *listsel);
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
	int				maxwordlen;
	int				amfiles;
	int				rows;
	int				cols;
	t_file_list		*listiter;

	listiter = *listsel;
	maxwordlen = find_longest_word(listbeg, &amfiles);
	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	while (cols--)
		*listsel = (*listsel)->prev;
	print_files(0, *listsel, 1, listiter);
	print_files(0, *listsel, 1, *listsel);
}

void			press_down(t_file_list *listbeg,
		t_file_list **listsel)
{
	int				maxwordlen;
	int				amfiles;
	int				rows;
	int				cols;
	t_file_list		*listiter;

	listiter = *listsel;
	maxwordlen = find_longest_word(listbeg, &amfiles);
	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	while (cols--)
		*listsel = (*listsel)->next;
	print_files(0, *listsel, 1, listiter);
	print_files(0, *listsel, 1, *listsel);
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

	tcgetattr(2, &tty);
	tty.c_lflag |= ICANON | ECHO;
	tcsetattr(2, TCSANOW, &tty);
	ft_fprintf(2, tgetstr("te", 0));
	ft_fprintf(2, tgetstr("ve", 0));
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
	print_files(0, 0, 0, 0);
}

void	winchange(int sig)
{
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

int		main(int argc, char **argv)
{
	t_file_list			*listsel;
	t_file_list			*listbeg;

	listsel = 0;
	term_associate();
	fill_list(argv, &listbeg);
	listsel = listbeg;
	set_noncanon();
	fill_signal_handlers();
	print_files(listbeg, listsel, 0, 0);
	cycle(&listbeg, &listsel);
	handle_exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:41:35 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	cycle(t_file_list **listbeg,
		t_file_list **listsel)
{
	char	buf[8];
	int		flag;

	flag = 0;
	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		if (!ft_strcmp(ESC, buf))
			quit_handler(0);
		else if (!ft_strcmp(ENTER, buf))
			press_return(*listbeg);
		else if (!ft_strcmp(UP, buf) || !ft_strcmp("k", buf))
			press_up(*listbeg, listsel, &flag);
		else if (!ft_strcmp(DOWN, buf) || !ft_strcmp("j", buf))
			press_down(*listbeg, listsel, &flag);
		else if (!ft_strcmp(LEFT, buf) || !ft_strcmp("h", buf))
			press_left(*listbeg, listsel, &flag);
		else if (!ft_strcmp(RIGHT, buf) || !ft_strcmp("l", buf))
			press_right(*listbeg, listsel, &flag);
		else if (!ft_strcmp(SPACE, buf))
			press_space(*listbeg, listsel, &flag);
		else if (!ft_strcmp(DEL, buf) || !ft_strcmp(BACKSPACE, buf))
			press_del(listbeg, listsel) && (flag = 1);
	}
}

int		main(int argc, char **argv)
{
	t_file_list			*listsel;
	t_file_list			*listbeg;

	if (argc == 1)
	{
		ft_fprintf(2, "ft_select:\nusage: ft_select [file ...]\n");
		return (1);
	}
	listsel = 0;
	term_associate();
	fill_list(argv, &listbeg);
	listsel = listbeg;
	set_noncanon();
	fill_signal_handlers();
	print_files(listbeg, listsel, 0, 0);
	cycle(&listbeg, &listsel);
	handle_exit();
}

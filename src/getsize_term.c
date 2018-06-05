/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getsize_term.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:20:33 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:21:01 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:21:33 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:21:36 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int				press_del(t_file_list **listbeg,
		t_file_list **listsel)
{
	t_file_list		*listnext;
	int				count;

	if ((*listbeg)->next == *listbeg)
		quit_handler(0);
	if (*listsel == *listbeg)
		*listbeg = (*listbeg)->next;
	listnext = (*listsel)->next;
	(*listsel)->next->prev = (*listsel)->prev;
	(*listsel)->prev->next = (*listsel)->next;
	(*listsel)->row = 0;
	(*listsel)->col = 0;
	free(*listsel);
	*listsel = listnext;
	print_files(*listbeg, *listsel, 0, 0);
	count = 0;
	return (1);
}

void			press_space(t_file_list *listbeg,
		t_file_list **listsel, int *flag)
{
	(*listsel)->ispressed = !(*listsel)->ispressed;
	(*listsel) = (*listsel)->next;
	if (!*flag)
	{
		print_files(0, *listsel, 1, (*listsel)->prev);
		print_files(0, *listsel, 1, *listsel);
	}
	else
	{
		print_files(listbeg, *listsel, 0, 0);
		*flag = 0;
	}
}

void			press_return(t_file_list *listbeg)
{
	int			count;
	int			firstword;
	t_file_list	*listiter;

	handle_exit();
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

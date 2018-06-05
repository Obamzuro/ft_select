/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrow_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:20:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:40:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void			press_right(t_file_list *listbeg,
		t_file_list **listsel, int *flag)
{
	*listsel = (*listsel)->next;
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

void			press_left(t_file_list *listbeg,
		t_file_list **listsel, int *flag)
{
	*listsel = (*listsel)->prev;
	if (!*flag)
	{
		print_files(0, *listsel, 1, (*listsel)->next);
		print_files(0, *listsel, 1, *listsel);
	}
	else
	{
		print_files(listbeg, *listsel, 0, 0);
		*flag = 0;
	}
}

void			press_up(t_file_list *listbeg,
		t_file_list **listsel, int *flag)
{
	t_file_list		*listiter;

	listiter = (*listsel)->prev;
	while (listiter->colnum != (*listsel)->colnum)
		listiter = listiter->prev;
	if (!*flag)
	{
		print_files(0, listiter, 1, *listsel);
		print_files(0, listiter, 1, listiter);
	}
	else
	{
		print_files(listbeg, listiter, 0, 0);
		*flag = 0;
	}
	*listsel = listiter;
}

void			press_down(t_file_list *listbeg,
		t_file_list **listsel, int *flag)
{
	t_file_list		*listiter;

	listiter = (*listsel)->next;
	while (listiter->colnum != (*listsel)->colnum)
		listiter = listiter->next;
	if (!*flag)
	{
		print_files(0, listiter, 1, *listsel);
		print_files(0, listiter, 1, listiter);
	}
	else
	{
		print_files(listbeg, listiter, 0, 0);
		*flag = 0;
	}
	*listsel = listiter;
}

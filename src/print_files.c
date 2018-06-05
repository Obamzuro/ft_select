/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:21:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:38:21 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void	get_cursor_position2(t_file_list *listiter)
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

static void	get_cursor_position(t_file_list *listiter)
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

static void	print_special_file(t_file_list *listiter,
		t_file_list *listsel)
{
	ft_fprintf(2, tgoto(tgetstr("cm", 0), listiter->col - 1,
				listiter->row - 1));
	print_files_style(listiter, listsel);
	ft_fprintf(2, listiter->name);
	ft_fprintf(2, DEFAULT);
}

static void	print_files_inner(t_file_list *listbeg,
		t_file_list *listsel, int maxwordlen, int amfiles)
{
	t_file_list		*listiter;
	int				cols;
	int				rows;
	int				count;

	if (get_tty_size(amfiles, &cols, &rows, maxwordlen))
		return ;
	listiter = listbeg;
	count = 0;
	while (listiter != listbeg || count == 0)
	{
		get_cursor_position(listiter);
		print_files_style(listiter, listsel);
		ft_fprintf(2, listiter->name);
		ft_fprintf(2, DEFAULT);
		print_files_padding(listiter, maxwordlen);
		listiter->rownum = count / cols;
		listiter->colnum = count % cols;
		if ((count + 1) % cols == 0)
			ft_fprintf(2, "\n");
		listiter = listiter->next;
		++count;
	}
}

void		print_files(t_file_list *plistbeg,
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

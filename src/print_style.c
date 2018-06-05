/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_style.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/05 12:22:05 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/05 12:22:23 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void			print_color_name_dir(struct stat *tempstat)
{
	if (tempstat->st_mode & 02 && tempstat->st_mode & 01000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BGREEN);
	else if (tempstat->st_mode & 02 && !(tempstat->st_mode & 01000))
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BYELLOWN);
	else
		ft_fprintf(2, ANSI_COLOR_BLUE);
}

static void			print_color_name_exec(struct stat *tempstat)
{
	if (tempstat->st_mode & 04000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BRED);
	else if (tempstat->st_mode & 02000)
		ft_fprintf(2, "%s%s", ANSI_COLOR_BLACK, ANSI_COLOR_BCYAN);
	else
		ft_fprintf(2, ANSI_COLOR_RED);
}

static void			print_color_name(struct stat *tempstat)
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

void				print_files_style(t_file_list *listiter,
		t_file_list *listsel)
{
	struct stat		tempstat;

	if (listiter->ispressed)
		ft_fprintf(2, tgetstr("mr", 0));
	if (listiter == listsel)
		ft_fprintf(2, tgetstr("us", 0));
	if (lstat(listiter->name, &tempstat) == -1)
		ft_fprintf(2, ANSI_COLOR_YELLOW);
	else
		print_color_name(&tempstat);
}

void				print_files_padding(t_file_list *listiter,
		int maxwordlen)
{
	int i;
	int offset;

	i = -1;
	offset = maxwordlen + 1 - ft_strlen(listiter->name);
	while (++i < offset)
		write(2, " ", 1);
}

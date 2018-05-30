/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sel_putchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 13:08:59 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/30 13:44:56 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static char	count_bytes(int a)
{
	if (a <= 0x7F)
		return (1);
	if (a <= 0x7FF)
		return (2);
	if (a <= 0xFFFF)
		return (3);
	return (4);
}

static void	print_for_two(int c)
{
	char	b;

	b = (char)(((c >> 6) & 0xDF) | 0xC0);
	ft_putchar(b);
	b = (char)((c & 0xBF) | 0x80);
	ft_putchar(b);
}

static void	print_for_three(int c)
{
	char	b;

	b = (char)(((c >> 12) & 0xEF) | 0xE0);
	ft_putchar(b);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	ft_putchar(b);
	b = (char)((c & 0xBF) | 0x80);
	ft_putchar(b);
}

static void	print_for_four(int c)
{
	char	b;

	b = (char)(((c >> 18) & 0xF7) | 0xF0);
	ft_putchar(b);
	b = (char)(((c >> 12) & 0xBF) | 0x80);
	ft_putchar(b);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	ft_putchar(b);
	b = (char)((c & 0xBF) | 0x80);
	ft_putchar(b);
}

int			sel_putchar(int c)
{
	char	b;

	b = count_bytes(c);
	if (b == 1)
		ft_putchar(c);
	else if (b == 2)
		print_for_two(c);
	else if (b == 3)
		print_for_three(c);
	else if (b == 4)
		print_for_four(c);
	return (b);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:19:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/28 16:19:31 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int main(int argc, char **argv)
{
	char	buf[1024];
	char	*termtype;
	int		success;

	termtype = getenv("TERM");
	if (!termtype)
	{
		ft_fprintf(2, "Couldn't find an env TERM\n");
		return (1);
	}
	success = tgetent(buf, termtype);
	if (success < 0)
	{
		ft_fprintf(2, "Couldn't access the termcap data base\n");
		return (1);
	}
	if (!success)
	{
		ft_fprintf(2, "Terminal type '%s' is not defined.\n", termtype);
		return (1);
	}
}

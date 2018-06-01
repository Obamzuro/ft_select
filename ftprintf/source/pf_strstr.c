/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 13:20:23 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/01 17:18:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		pf_strstr(const char *big, const char *little)
{
	while (*big == *little && *big && *little)
	{
		++big;
		++little;
	}
	if (!*little)
		return (1);
	return (0);
}

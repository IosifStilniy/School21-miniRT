/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:34:22 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/01 22:03:42 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoi(const char *num)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	if (!num)
		return (0);
	while ((num[i] > 8 && num[i] < 14) || num[i] == 32)
		i++;
	if (num[i] == 43 || num[i] == 45)
	{
		if (num[i] == 45)
			sign = -1;
		i++;
	}
	while (num[i] > 47 && num[i] < 58)
	{
		result = result * 10 + num[i] - 48;
		i++;
	}
	return (result * sign);
}

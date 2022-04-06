/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:34:47 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 18:34:47 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_str(int n, char *str, size_t i)
{
	int	sign;

	sign = 1;
	if (n < 0)
	{
		str[0] = '-';
		sign = -1;
	}
	if (n / 10)
		ft_putnbr_str(sign * (n / 10), str, i - 1);
	str[i] = sign * (n % 10) + 48;
}

static	size_t	ft_len(int n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (1);
	if (n < 0)
		i++;
	while (n)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	size_t	size;
	char	*str;

	size = ft_len(n);
	str = (char *)malloc((size + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_putnbr_str(n, str, size - 1);
	str[size] = 0;
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:36:04 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 18:36:05 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	p1;
	unsigned char	p2;
	size_t			i;

	i = -1;
	while (++i < n && (s1[i] || s2[i]))
	{
		p1 = s1[i];
		p2 = s2[i];
		if ((int)p1 - (int)p2)
			return ((int)p1 - (int)p2);
	}
	return (0);
}

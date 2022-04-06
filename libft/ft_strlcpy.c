/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:35:56 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 18:35:56 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size)
	{
		i = -1;
		while (++i < size - 1 && src[i])
			dst[i] = src[i];
		dst[i] = 0;
	}
	if (src[i] == 0)
		return (i);
	return (ft_strlen(src));
}

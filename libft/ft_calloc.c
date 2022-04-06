/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:34:28 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 18:34:28 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*arr;
	size_t	i;

	i = -1;
	arr = malloc(n * size);
	if (!arr)
		return (NULL);
	while (++i < n * size)
		*(char *)(arr + i) = 0;
	return (arr);
}

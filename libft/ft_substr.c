/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:26:59 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 19:47:32 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	size;

	i = -1;
	if (!s)
		size = 0;
	else
		size = ft_strlen(s);
	if (size < start)
		len = 0;
	else if (size - start < len)
		len = size;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (++i < len)
		str[i] = s[start + i];
	str[i] = 0;
	return (str);
}

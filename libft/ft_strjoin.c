/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:35:51 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/14 18:47:12 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*str;

	i = ft_strlen(s1);
	j = ft_strlen(s2);
	str = (char *)malloc((i + j + 1) * sizeof(char));
	if (!str)
		return (0);
	i = -1;
	while (s1 && s1[++i])
		str[i] = s1[i];
	j = -1;
	while (s2 && s2[++j])
		str[i + j] = s2[j];
	str[i + j] = 0;
	return (str);
}

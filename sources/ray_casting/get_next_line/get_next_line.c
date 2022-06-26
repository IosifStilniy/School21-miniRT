/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 16:45:22 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/18 22:58:03 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	check_buffer(char *buf, char **result, char **newline_pointer)
{
	char	*previous_newline;

	if (buf[0])
	{
		previous_newline = ft_strchr(buf, '\n');
		*result = add_buffer(*result, previous_newline + 1);
		*(previous_newline) = 'a';
		*newline_pointer = ft_strchr(*result, '\n');
	}
}

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*result;
	char		*newline_pointer;
	int			ret;

	ret = 1;
	newline_pointer = NULL;
	result = NULL;
	check_buffer(buf, &result, &newline_pointer);
	while (ret && !newline_pointer)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		buf[ret] = 0;
		result = add_buffer(result, buf);
		if (ret == -1 || (!ret && !result) || (result && !result[0]))
		{
			free(result);
			return (NULL);
		}
		newline_pointer = ft_strchr(result, '\n');
	}
	if (newline_pointer)
		*(newline_pointer + 1) = 0;
	return (result);
}

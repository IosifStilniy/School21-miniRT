/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 16:45:22 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/02 22:37:12 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	check_buffer(char *buf, char **result, char **newline_pointer)
{
	char	*previous_newline;

	if (buf && buf[0])
	{
		previous_newline = ft_strchr(buf, '\n');
		*result = add_buffer(*result, previous_newline + 1);
		*(previous_newline) = 'a';
		*newline_pointer = ft_strchr(*result, '\n');
	}
}

static void	move_buffer(int fd, int *ret, char **buf_fd, char **result)
{
	if ((*buf_fd) && buf_fd[0])
	{
		free((*buf_fd));
		(*buf_fd) = NULL;
	}
	*buf_fd = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!(*buf_fd))
	{
		*ret = 0;
		return ;
	}
	*ret = read(fd, *buf_fd, BUFFER_SIZE);
	if (*ret < 0)
		return ;
	(*buf_fd)[*ret] = 0;
	*result = add_buffer(*result, (*buf_fd));
}

static void	*get_rid_of_everything(char **buf, char **result)
{
	if (buf && *buf)
	{
		free(*buf);
		*buf = NULL;
	}
	if (result && *result)
	{
		free(*result);
		*result = NULL;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*buf[OPEN_MAX];
	char		*result;
	char		*newline_pointer;
	int			ret;

	ret = 1;
	newline_pointer = NULL;
	result = NULL;
	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	check_buffer(buf[fd], &result, &newline_pointer);
	while (ret && !newline_pointer)
	{
		move_buffer(fd, &ret, &buf[fd], &result);
		if (ret < 0 || (!ret && !result) || (result && !result[0]))
			return (get_rid_of_everything(&buf[fd], &result));
		newline_pointer = ft_strchr(result, '\n');
	}
	if (!newline_pointer)
		get_rid_of_everything(&buf[fd], NULL);
	if (newline_pointer)
		*(newline_pointer + 1) = 0;
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:21:33 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/24 20:49:01 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_info	*ft_init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->light = NULL;
	info->object = NULL;
	info->camera = NULL;
	info->mlx_ptr = NULL;
	info->a_light = NULL;
	info->wnd_ptr = NULL;
	info->data.img = NULL;
	info->data.addr = NULL;
	info->total = 0;
	return (info);
}

static int	ft_check_symbols(char **piece)
{
	int	i;
	int	j;

	i = -1;
	while (piece[++i])
	{
		j = -1;
		while (piece[i][++j])
			if (!ft_strchr("+-0123456789.,\n", piece[i][j]))
				return (1);
	}
	return (0);
}

static int	ft_fill_information(char **piece, t_info *info)
{
	if (ft_strlen(piece[0]) > 2 || ft_check_symbols(&piece[1]))
		return (1);
	if (ft_strlen(piece[0]) == 1)
	{
		if (piece[0][0] == 'A')
			return (ft_fill_amb_light_info(piece, info));
		if (piece[0][0] == 'C')
			return (ft_fill_camera_info(piece, info));
		if (piece[0][0] == 'L')
			return (ft_fill_light_info(piece, info));
	}
	else
	{
		if (!ft_strncmp("sp", piece[0], 2))
			return (ft_fill_sphere_info(piece, info));
		if (!ft_strncmp("pl", piece[0], 2))
			return (ft_fill_plane_info(piece, info));
		if (!ft_strncmp("cy", piece[0], 2))
			return (ft_fill_cylinder_info(piece, info));
	}
	return (1);
}

static int	ft_read_information(int fd, t_info *info)
{
	char	*line;
	char	**piece;

	line = NULL;
	while (1)
	{
		piece = NULL;
		line = get_next_line(fd);
		if (!line)
			break ;
		piece = ft_split(line, ' ');
		if (!piece)
			return (1);
		if (line)
			free(line);
		if (piece[0] && piece[0][0] != '\n' && ft_fill_information(piece, info))
			return (ft_clear_char_array(piece));
		ft_clear_char_array(piece);
	}
	return (0);
}

t_info	*ft_validate_file(char *filename)
{
	t_info	*info;
	int		len;
	int		fd;

	fd = 0;
	len = ft_strlen(filename);
	if (len < 4 || ft_strrchr(filename, 't') != &filename[len - 1]
		|| ft_strrchr(filename, 'r') != &filename[len - 2]
		|| ft_strrchr(filename, '.') != &filename[len - 3])
		return (NULL);
	if (len < 5 && ft_strrchr(filename, '/') == &filename[len - 4])
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	info = ft_init_info();
	if (info && ft_read_information(fd, info))
	{
		close(fd);
		ft_clear_info(info);
		return (NULL);
	}
	close(fd);
	return (info);
}

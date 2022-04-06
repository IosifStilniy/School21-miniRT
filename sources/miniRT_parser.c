/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:21:33 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/03 22:19:41 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

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

int	ft_read_information(int fd, t_info *info)
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

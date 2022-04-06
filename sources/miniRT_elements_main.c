/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_elements_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 19:09:44 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/03 20:40:35 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	ft_fill_amb_light_info(char **piece, t_info *info)
{
	if (info->a_light || !piece[1] || !piece[2])
		return (1);
	info->a_light = (t_amlight *)malloc(sizeof(t_amlight));
	if (!info->a_light)
		return (1);
	info->a_light->light_ratio = ft_atoidb(piece[1]);
	if (info->a_light->light_ratio > 1.0 || info->a_light->light_ratio < 0.0)
		return (1);
	if (ft_get_color_values(piece[2], &info->a_light->color))
		return (1);
	if (piece[3] && piece[3][0] != '\n')
		return (1);
	return (0);
}

int	ft_fill_camera_info(char **piece, t_info *info)
{
	if (info->camera || !piece[1] || !piece[2] || !piece[3])
		return (1);
	info->camera = (t_camera *)malloc(sizeof(t_camera));
	if (!info->camera)
		return (1);
	if (ft_get_position_values(piece[1], &info->camera->position))
		return (1);
	if (ft_get_vector_values(piece[2], &info->camera->vector))
		return (1);
	info->camera->fov = ft_atoi(piece[3]);
	if (info->camera->fov > 180 || info->camera->fov < 0)
		return (1);
	if (piece[4] && piece[4][0] != '\n')
		return (1);
	return (0);
}

int	ft_fill_light_info(char **piece, t_info *info)
{
	if (info->light || !piece[1] || !piece[2] || !piece[3])
		return (1);
	info->light = (t_light *)malloc(sizeof(t_light));
	if (!info->light)
		return (1);
	if (ft_get_position_values(piece[1], &info->light->position))
		return (1);
	info->light->light_ratio = ft_atoidb(piece[2]);
	if (info->light->light_ratio > 1.0 || info->light->light_ratio < 0.0)
		return (1);
	if (ft_get_color_values(piece[3], &info->light->color))
		return (1);
	if (piece[4] && piece[4][0] != '\n')
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_elements_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 19:09:44 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/28 19:46:27 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

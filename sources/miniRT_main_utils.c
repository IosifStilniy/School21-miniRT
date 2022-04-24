/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 18:26:11 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/24 20:49:17 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_clear_info(t_info *info)
{
	t_obj	*curr;

	if (!info)
		return (1);
	if (info->a_light)
		free(info->a_light);
	if (info->camera)
		free(info->camera);
	if (info->light)
		free(info->light);
	curr = NULL;
	while (info->object)
	{
		curr = info->object->next;
		free(info->object->identifier);
		free(info->object);
		info->object = curr;
	}
	free(info);
	return (1);
}

int	ft_exit(void)
{
	ft_putendl_fd(ESC_TEXT, 1);
	exit(0);
}

int	ft_key_hook(int keycode, t_info *info)
{
	if (keycode == 53)
		ft_exit();
	(void)info;
	return (0);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

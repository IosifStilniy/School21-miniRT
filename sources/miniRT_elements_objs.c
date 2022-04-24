/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_elements_objs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 19:10:56 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/24 20:49:22 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	ft_iterate_objects(t_info *info, t_obj *obj)
{
	t_obj	*curr;

	if (!info->object)
		info->object = obj;
	else
	{
		curr = info->object;
		while (curr->next)
			curr = curr->next;
		curr->next = obj;
	}
}

int	ft_fill_sphere_info(char **piece, t_info *info)
{
	t_obj	*obj;

	if (!piece[1] || !piece[2] || !piece[3])
		return (1);
	obj = (t_obj *)malloc(sizeof(t_obj));
	if (!obj)
		return (1);
	ft_memset(obj, 0, sizeof(t_obj));
	ft_iterate_objects(info, obj);
	obj->identifier = ft_strdup(piece[0]);
	if (ft_get_position_values(piece[1], &obj->position))
		return (1);
	obj->diameter = ft_atoidb(piece[2]);
	if (obj->diameter < 0)
		return (1);
	if (ft_get_color_values(piece[3], &obj->color))
		return (1);
	if (piece[4] && piece[4][0] != '\n')
		return (1);
	++info->total;
	return (0);
}

int	ft_fill_plane_info(char **piece, t_info *info)
{
	t_obj	*obj;

	if (!piece[1] || !piece[2] || !piece[3])
		return (1);
	obj = (t_obj *)malloc(sizeof(t_obj));
	if (!obj)
		return (1);
	ft_memset(obj, 0, sizeof(t_obj));
	ft_iterate_objects(info, obj);
	obj->identifier = ft_strdup(piece[0]);
	if (ft_get_position_values(piece[1], &obj->position))
		return (1);
	if (ft_get_vector_values(piece[2], &obj->vector))
		return (1);
	if (ft_get_color_values(piece[3], &obj->color))
		return (1);
	if (piece[4] && piece[4][0] != '\n')
		return (1);
	++info->total;
	return (0);
}

int	ft_fill_cylinder_info(char **piece, t_info *info)
{
	t_obj	*obj;

	if (!piece[1] || !piece[2] || !piece[3] || !piece[4] || !piece[5])
		return (1);
	obj = (t_obj *)malloc(sizeof(t_obj));
	if (!obj)
		return (1);
	ft_memset(obj, 0, sizeof(t_obj));
	ft_iterate_objects(info, obj);
	obj->identifier = ft_strdup(piece[0]);
	if (ft_get_position_values(piece[1], &obj->position))
		return (1);
	if (ft_get_vector_values(piece[2], &obj->vector))
		return (1);
	obj->diameter = ft_atoidb(piece[3]);
	obj->height = ft_atoidb(piece[4]);
	if (obj->diameter < 0 || obj->height < 0)
		return (1);
	if (ft_get_color_values(piece[5], &obj->color))
		return (1);
	if (piece[6] && piece[6][0] != '\n')
		return (1);
	++info->total;
	return (0);
}

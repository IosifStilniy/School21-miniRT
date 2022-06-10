/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/10 19:19:03 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_cart	ft_cast_ray(float x, float y, float z)
{
	t_cart	ray;

	ray.x = x;
	ray.y = y;
	ray.z = z;
	return (ft_get_vector_norm(ray, ft_get_vector_length(ray)));
}

static int	ft_find_shapes(t_info *info, t_cart ray, t_list *object)
{
	t_obj	*closest_object;
	float	temp_distance;
	float	distance;

	distance = INFINITY;
	while (object)
	{
		// if (!ft_strncmp("sp", object->identifier, 3))
		temp_distance = ft_get_intersection_with_sphere(ray,
					(t_cart){0, 0, 0}, object->content);
		// else if (!ft_strncmp("pl", object->identifier, 3))
		// 	temp_distance = ft_get_intersection_with_plane(ray,
		// 			info->camera->position, object);
		if (temp_distance < distance)
		{
			distance = temp_distance;
			closest_object = object->content;
		}
		object = object->next;
	}
	if (distance != INFINITY)
		return (ft_find_light(ft_multiply_vector(ray, distance),
				closest_object, info));
	return (0x00000000);
}

static void	ft_raytracing_algorithm(t_info *info)
{
	t_cart	screen_pixel;
	t_cart	ray;
	int		color;

	screen_pixel.z = info->win.camera.focus;
	screen_pixel.y = -1;
	while (++screen_pixel.y < RESY)
	{
		screen_pixel.x = -1;
		while (++screen_pixel.x < RESX)
		{
			ray = ft_cast_ray(screen_pixel.x - info->win.cntr.x, info->win.cntr.y - screen_pixel.y, screen_pixel.z);
			color = ft_find_shapes(info, ray, info->win.camera.objs);
			my_mlx_pixel_put(&info->data, screen_pixel.x,
				screen_pixel.y, color);
		}
	}
}

void	ft_draw_screen(t_info *info)
{
	ft_raytracing_algorithm(info);
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/16 22:10:01 by dcelsa           ###   ########.fr       */
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

static	float	ft_get_sphere_cone_phit(t_cart ray, t_polys *polys, t_vrtx *dots, t_poly *closest_poly)
{
	int		i;
	t_cart	p[3];
	float	distance;
	float	temp_distance;

	i = -1;
	distance = INFINITY;
	while (++i < polys->polynum)
	{
		temp_distance = ft_get_intersection_with_poly(ray, (t_cart){0, 0, 0}, polys->poly[i].norm, dots[polys->poly[i].dots[0]].dot);
		if (temp_distance < distance)
		{
			p[0] = dots[polys->poly[i].dots[0]].dot;
			p[1] = dots[polys->poly[i].dots[1]].dot;
			p[2] = dots[polys->poly[i].dots[2]].dot;
			if (ft_get_intersection_with_triangle(p, ft_multiply_vector(ray, temp_distance), polys->poly[i].norm))
			{
				distance = temp_distance;
				*closest_poly = polys->poly[i];
			}
		}
	}
	return (distance);
}

static int	ft_find_shapes(t_info *info, t_cart ray, t_list *object)
{
	t_obj	*current;
	float	distance;
	t_cart	closest_color;
	t_poly	closest_poly;

	distance = INFINITY;
	while (object)
	{
		current = object->content;
		if (current->dots.dotsnum != 0 && ft_get_intersection_with_sphere(ray, (t_cart){0, 0, 0}, object->content) < distance)
		{
			distance = ft_get_sphere_cone_phit(ray, &current->polys, current->dots.pos, &closest_poly);
			closest_color = current->colrs;
		}
		object = object->next;
	}
	if (distance != INFINITY)
		return (ft_find_light(ft_multiply_vector(ray, distance - 0.0001f), closest_poly.norm, closest_color, info));
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
	info->data.img = mlx_new_image(info->mlx_ptr, info->data.res.x, info->data.res.y);
	info->data.addr = mlx_get_data_addr(info->data.img, &info->data.bits_per_pixel, &info->data.line_length, &info->data.endian);
	ft_raytracing_algorithm(info);
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, info->win.cntr.x - info->data.cntr.x, info->win.cntr.y - info->data.cntr.y);
	mlx_destroy_image(info->mlx_ptr, info->data.img);
}

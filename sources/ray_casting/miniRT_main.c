/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 22:26:25 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#include "minirt.h"

void	ft_cast_ray(t_ray *ray, t_cart *direction, t_cart *origin)
{
	ray->dir = *direction;
	ray->orig = *origin;
	ft_vectnorm(&ray->dir);
}

static void	ft_trace_color(unsigned int *color, t_ray ray,
			t_list *objects, t_info *info)
{
	t_obj	*obj;
	float	dist[2];
	t_cart	nc0[3];
	t_cart	nc1[3];

	dist[0] = INFINITY;
	dist[1] = INFINITY;
	while (objects)
	{
		obj = (t_obj *)objects->content;
		if (obj->dots.dotsnum && ft_hit_sphere(ray, obj) < INFINITY)
			ft_hit_poly(ray, nc1, obj, &dist[1]);
		else if (!obj->dots.dotsnum)
			ft_hit_plane(ray, nc1, obj, &dist[1]);
		if (dist[1] < dist[0])
		{
			dist[0] = dist[1];
			nc0[1] = nc1[1];
			nc0[0] = nc1[0];
			nc0[2].x = obj->dots.dotsnum;
		}
		objects = objects->next;
	}
	ft_multvect(&ray.dir, dist[0], &nc1[2]);
	ft_shadowing(color, &nc1[2], nc0, info);
}

static void	ft_raytracing_algorithm(t_info *info)
{
	t_ray			ray;
	t_cart			origin;
	t_cart			direction;
	t_cart			pixel;
	unsigned int	color;

	origin.x = 0.0f;
	origin.y = 0.0f;
	origin.z = 0.0f;
	pixel.z = info->win.camera->focus;
	pixel.y = -1;
	while (++pixel.y < RESY)
	{
		pixel.x = -1;
		while (++pixel.x < RESX)
		{
			direction.x = pixel.x - info->win.cntr.x;
			direction.y = pixel.y - info->win.cntr.y;
			direction.z = pixel.z;
			ft_cast_ray(&ray, &direction, &origin);
			ft_trace_color(&color, ray, info->win.camera->objs, info);
			my_mlx_pixel_put(&info->data, pixel.x, pixel.y, color);
		}
	}
}

void	ft_draw_screen(t_info *info)
{
	mlx_destroy_image(info->mlx_ptr, info->data.img);
	info->data.img = mlx_new_image(info->mlx_ptr,
			info->data.res.x, info->data.res.y);
	info->data.addr = mlx_get_data_addr(info->data.img,
			&info->data.bits_per_pixel, &info->data.line_length,
			&info->data.endian);
	ft_bzero(info->data.addr, info->data.line_length * info->data.res.y);
	if (info->keybrd.render)
		ft_raytracing_algorithm(info);
	else
		framepic(&info->win, info->keybrd.normalpaint,
			info->win.camera->objs, &info->data);
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img,
		info->win.cntr.x - info->data.cntr.x,
		info->win.cntr.y - info->data.cntr.y);
	if (!info->keybrd.render)
		mlx_string_put(info->mlx_ptr, info->win.win, 8, 15,
			FRAMECLR, info->camtext);
}

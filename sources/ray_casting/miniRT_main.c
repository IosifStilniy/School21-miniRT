// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   miniRT_main.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
// /*   Updated: 2022/06/30 18:33:07 by dcelsa           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "minirt.h"

// void	ft_cast_ray(t_ray *ray, t_cart *direction, t_cart *origin)
// {
// 	ray->dir = *direction;
// 	ray->orig = *origin;
// 	ray->dir = ft_get_vector_norm(&ray->dir, ft_get_vector_length(&ray->dir));
// }

// static void	ft_get_color_from_shapes(t_ui *color, t_ray ray,
// 			t_list *objects, t_info *info)
// {
// 	t_obj	*obj;
// 	float	dist[2];
// 	t_cart	c_norm;
// 	t_cart	c_phit;
// 	t_cart	c_color;

// 	dist[0] = INFINITY;
// 	while (objects)
// 	{
// 		obj = (t_obj *)objects->content;
// 		if (obj->dots.dotsnum && ft_intersect_sphere(ray, obj) < INFINITY)
// 			ft_intersect_polygon(ray, &c_norm, obj, &dist[1]);
// 		else if (!obj->dots.dotsnum)
// 			ft_intersect_plane(ray, &obj->crdstm.oz.vector, &obj->crdstm.pos, &dist[1]);
// 		if (dist[1] < dist[0])
// 		{
// 			dist[0] = dist[1];
// 			c_color = *obj->colrs;
// 			if (!obj->dots.dotsnum)
// 				c_norm = obj->crdstm.oz.vector;
// 		}
// 		objects = objects->next;
// 	}
// 	c_phit = ft_multiply_vector(&ray.dir, dist[0]);
// 	*color = ft_shadowing(&c_phit, &c_norm, &c_color, info);
// }

// static void	ft_raytracing_algorithm(t_info *info)
// {
// 	t_ray	ray;
// 	t_cart	origin;
// 	t_cart	direction;
// 	t_cart	pixel;
// 	t_ui	color;

// 	origin.x = 0.0f;
// 	origin.y = 0.0f;
// 	origin.z = 0.0f;
// 	pixel.z = info->win.camera.focus;
// 	pixel.y = -1;
// 	while (++pixel.y < RESY)
// 	{
// 		pixel.x = -1;
// 		while (++pixel.x < RESX)
// 		{
// 			direction.x = pixel.x - info->win.cntr.x;
// 			direction.y = pixel.y - info->win.cntr.y;
// 			direction.z = pixel.z;
// 			ft_cast_ray(&ray, &direction, &origin);
// 			ft_get_color_from_shapes(&color, ray, info->win.camera.objs, info);
// 			my_mlx_pixel_put(&info->data, pixel.x, pixel.y, color);
// 		}
// 	}
// }

void	ft_draw_screen(t_info *info)
{
	mlx_destroy_image(info->mlx_ptr, info->data.img);
	info->data.img = mlx_new_image(info->mlx_ptr, info->data.res.x, info->data.res.y);
	info->data.addr = mlx_get_data_addr(info->data.img, &info->data.bits_per_pixel, &info->data.line_length, &info->data.endian);
	ft_bzero(info->data.addr, info->data.line_length * info->data.res.y);
	// if (info->keybrd.render)
	// 	ft_raytracing_algorithm(info);
	// else
	framepic(info->win.camera, &info->win.cntr, info->win.camera->objs, &info->data);
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, info->win.cntr.x - info->data.cntr.x, info->win.cntr.y - info->data.cntr.y);
	// if (!info->keybrd.render)
		mlx_string_put(info->mlx_ptr, info->win.win, 8, 15, FRAMECLR, info->camtext);
}

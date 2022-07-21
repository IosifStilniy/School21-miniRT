/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/03 23:11:12 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	ft_trace_color(unsigned int *color, t_ray ray,
			t_list *objects, t_info *info)
{
	t_obj	*obj;
	t_cart	c_phit;
	float	dist[2];
	t_cart	nc0[2];
	t_cart	nc1[2];

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
		}
		objects = objects->next;
	}
	ft_multvect(&ray.dir, dist[0], &c_phit);
	ft_shadowing(color, &c_phit, nc0, info);
}

static void *ft_raytracing_algorithm(void *thread)
{
	t_ray			ray;
	t_cart			origin;
	t_cart			direction;
	t_cart			pixel;
	unsigned int	color;

	origin.x = 0.0f;
	origin.y = 0.0f;
	origin.z = 0.0f;
	pixel.z = ((t_thrcast *)thread)->info->win.camera->focus;
	pixel.y = -1;
	while (++pixel.y < RESY)
	{
		pixel.x = ((t_thrcast *)thread)->leftx - 1;
		while (++pixel.x < ((t_thrcast *)thread)->rightx)
		{
			direction.x = pixel.x - ((t_thrcast *)thread)->info->win.cntr.x;
			direction.y = pixel.y - ((t_thrcast *)thread)->info->win.cntr.y;
			direction.z = pixel.z;
			ft_cast_ray(&ray, &direction, &origin);
			ft_trace_color(&color, ray,
				((t_thrcast *)thread)->info->win.camera->objs,
				((t_thrcast *)thread)->info);
			my_mlx_pixel_put(&((t_thrcast *)thread)->info->data, pixel.x,
				pixel.y, color);
		}
	}
	((t_thrcast *)thread)->done++;
	return (NULL);
}

static void	threadinit(int leftx, int rightx, t_info *info, t_thrcast *thread)
{
	thread->leftx = leftx;
	thread->rightx = rightx;
	thread->done = FALSE;
	thread->info = info;
	pthread_create(&thread->thread, NULL, &ft_raytracing_algorithm, thread);
	pthread_detach(thread->thread);
}

static void	multithreadcasting(t_info *info)
{
	t_thrcast	threads[THREADCOUNT];
	int			done;
	int			leftx;
	int			rightx;
	int			i;

	leftx = 0;
	rightx = RESX / THREADCOUNT;
	i = -1;
	while (++i < THREADCOUNT - 1)
	{
		threadinit(leftx, rightx, info, &threads[i]);
		leftx += RESX / THREADCOUNT;
		rightx += RESX / THREADCOUNT;
	}
	threadinit(leftx, RESX - leftx, info, &threads[i]);
	done = 0;
	while (done < THREADCOUNT)
	{
		done = 0;
		i = -1;
		while (++i < THREADCOUNT)
			done += threads[i].done;
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
		multithreadcasting(info);
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

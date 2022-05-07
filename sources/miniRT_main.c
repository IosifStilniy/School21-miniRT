/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/05/07 12:45:23 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	ft_create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

static double	ft_get_vector_length(t_cords vector)
{
	return (sqrt(vector.x * vector.x + vector.y * vector.y
			+ vector.z * vector.z));
}

static void	ft_get_distance_to_sphere(t_cords point, t_cords view,
	t_obj *sphere, t_cords *cords)
{
	t_cords	vector_norm;
	t_cords	vector_summ;
	t_cords	vector_prod;
	double	length_of_prod;
	double	length_of_norm;
	double	length_of_small;
	double	length_of_total;
	double	length_to_sphere;
	double	percentage;

	vector_norm.x = point.x - view.x;
	vector_norm.y = point.y - view.y;
	vector_norm.z = point.z - view.z;
	vector_summ.x = -view.x + sphere->position.x;
	vector_summ.y = -view.y + sphere->position.y;
	vector_summ.z = -view.z + sphere->position.z;
	vector_prod.x = vector_summ.y * vector_norm.z
		- vector_summ.z * vector_norm.y;
	vector_prod.y = vector_summ.x * vector_norm.z
		- vector_summ.z * vector_norm.x;
	vector_prod.z = vector_summ.x * vector_norm.y
		- vector_summ.y * vector_norm.x;
	length_of_prod = ft_get_vector_length(vector_prod);
	length_of_norm = ft_get_vector_length(vector_norm);
	length_of_prod /= length_of_norm;
	if (length_of_prod > sphere->diameter / 2)
		return ;
	length_to_sphere = ft_get_vector_length(sphere->position);
	length_of_small = sqrt(sphere->diameter / 2 * sphere->diameter / 2 - length_of_prod * length_of_prod);
	length_of_total = sqrt(length_to_sphere * length_to_sphere - length_of_prod * length_of_prod);
	length_to_sphere = ft_get_vector_length(point);
	percentage = (length_of_total - length_of_small) / length_of_total;
	(*cords).x = point.x / length_to_sphere * percentage;
	(*cords).y = point.y / length_to_sphere * percentage;
	(*cords).z = point.z / length_to_sphere * percentage;
}

static void	ft_get_distance_to_plane(t_cords point, t_cords view,
	t_obj *plane, t_cords *cords)
{
	t_cords	vector_norm;
	double	upside;
	double	downside;

	vector_norm.x = point.x - view.x;
	vector_norm.y = point.y - view.y;
	vector_norm.z = point.z - view.z;
	upside = plane->vector.x * plane->position.x + plane->vector.y
		* plane->position.y + plane->vector.z * plane->position.z
		+ plane->vector.x * view.x + plane->vector.y
		* view.y + plane->vector.z * view.z;
	downside = vector_norm.x * plane->vector.x + vector_norm.y
		* plane->vector.y + vector_norm.z * plane->vector.z;
	if (!upside || !downside)
		return ;
	vector_norm.x *= (upside / downside) + view.x;
	vector_norm.y *= (upside / downside) + view.y;
	vector_norm.z *= (upside / downside) + view.z;
	if (vector_norm.x < view.x || vector_norm.y < -512
		|| vector_norm.y > 512 || vector_norm.z < -384
		|| vector_norm.z > 384)
		return ;
	(*cords).x = vector_norm.x;
	(*cords).y = vector_norm.y;
	(*cords).z = vector_norm.z;
}

static void	ft_ray_tracer(t_info *info)
{
	double	distance_to_object;
	double	distance_to_closest;
	t_cords	point;
	t_cords	cords;
	t_obj	*copy;
	int		color;

	point.x = 512 * sin((90 - (info->camera->fov / 2)) * M_PI / 180)
		/ sin((info->camera->fov / 2) * M_PI / 180);
	point.z = -385;
	while (++point.z <= 384)
	{
		point.y = -513;
		while (++point.y <= 512)
		{
			distance_to_closest = INFINITY;
			copy = info->object;
			color = 0x00000000;
			while (copy)
			{
				cords.x = INFINITY;
				cords.y = INFINITY;
				cords.z = INFINITY;
				if (!ft_strncmp(copy->identifier, "sp", 3))
				{
					ft_get_distance_to_sphere(point, info->camera->position, copy, &cords);
					distance_to_object = ft_get_vector_length(cords);
					if (distance_to_object < distance_to_closest)
					{
						distance_to_closest = distance_to_object;
						color = ft_create_trgb(0, copy->color.r, copy->color.g, copy->color.b);
					}
				}
				else if (!ft_strncmp(copy->identifier, "pl", 3))
				{
					ft_get_distance_to_plane(point, info->camera->position, copy, &cords);
					distance_to_object = ft_get_vector_length(cords);
					if (distance_to_object != INFINITY)
					{
						if (distance_to_closest == INFINITY || distance_to_object - distance_to_closest < 1)
						{
							distance_to_closest = distance_to_object;
							color = ft_create_trgb(0, copy->color.r, copy->color.g, copy->color.b);
						}
					}
				}
				copy = copy->next;
			}
			if (distance_to_closest == INFINITY)
				my_mlx_pixel_put(&info->data, point.y + 512, point.z + 384, color);
			else
				my_mlx_pixel_put(&info->data, point.y + 512, point.z + 384, color);
		}
	}
}

static void	backwards_ray_tracing(t_info *info)
{
	info->mlx_ptr = mlx_init();
	info->wnd_ptr = mlx_new_window(info->mlx_ptr, 1024, 768, "MiniRT");
	info->data.img = mlx_new_image(info->mlx_ptr, 1024, 768);
	info->data.addr = mlx_get_data_addr(info->data.img,
			&info->data.bits_per_pixel, &info->data.line_length,
			&info->data.endian);
	ft_ray_tracer(info);
	mlx_put_image_to_window(info->mlx_ptr, info->wnd_ptr, info->data.img, 0, 0);
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_PRESS, 0, &ft_key_hook, info);
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_EXIT, 0, &ft_exit, NULL);
	mlx_loop(info->mlx_ptr);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int		fd;

	info.prog = ft_strrchr(*argv++, '/');
	if (argc != 2)
		customerr(info.prog, INVINP, FALSE);
	fd = file_check(*argv, info.prog);
	info.a_light.determined = FALSE;
	info.lights.determined = FALSE;
	info.win.camera.determined = FALSE;
	ft_read_information(fd, &info);
	backwards_ray_tracing(info);
	return (0);
}

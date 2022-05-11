/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/05/11 22:05:38 by dcelsa           ###   ########.fr       */
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

void	wininit(t_win *win, void *mlx, char *prog, char *file)
{
	char	*buf;

	buf = ft_strjoin(prog, ": ");
	win->header = ft_strjoin(buf, file);
	free(buf);
	win->res.x = RESX;
	win->res.y = RESY;
	win->win = mlx_new_window(mlx, win->res.x, win->res.y, win->header);
	win->cntr.x = win->res.x / 2;
	win->cntr.y = win->res.y / 2;
}

t_bool	objinframe(t_obj *obj, t_camera *camera, t_res *cntr)
{
	t_axis	v1;
	t_axis	v2;
	t_axis	edge;
	t_axis	visor;

	vectortoobj(&camera->pos, &obj->crdstm.pos, &v1);
	axisbuilder(&v1, &camera->crdstm.oz, &v2);
	if (v2.ang > M_PI_2 - 0.001)
		return (v1.length < obj->outframe);
	vectorsizing(v1.length * cosf(v2.ang), &camera->crdstm.oz.vector, &v2);
	vectortoobj(&v1.vector, &v2.vector, &edge);
	vectorsizing(obj->outframe, &edge.vector, &edge);
	vectortoobj(&camera->pos, vectodot(&edge.vector, &obj->crdstm.pos), &visor);
	axisbuilder(&visor, &camera->crdstm.ox, &v1);
	vectorsizing(visor.length * cosf(v1.ang), &camera->crdstm.ox, &v1);
	axisbuilder(&visor, &camera->crdstm.oz, &edge);
	v1.length *= camera->focus / (visor.length * cosf(edge.ang));
	if (fabsf(v1.length) > cntr->x)
		return (FALSE);
	axisbuilder(&visor, &camera->crdstm.oy, &v2);
	vectorsizing(visor.length * cosf(v2.ang), &camera->crdstm.oy, &v2);
	v2.length *= camera->focus / (visor.length * cosf(edge.ang));
	return (fabsf(v2.length) < cntr->y);
}

void	definecamobj(t_obj *ref, t_obj *obj, t_camera *camera)
{
	int		j;

	j = -1;
	while (++j < obj->polys.polynum)
		cartcopy(&ref->polys.poly[j].norm.vector, &obj->polys.polynorms[j], 1);
	cartcopy(&ref->crdstm.pos, &obj->crdstm.pos, 1);
	cartcopy(&obj->dots.dots, &obj->dots.pos, obj->dots.dotsnum);
	computeworldcoords(&obj->dots, &obj->polys, &obj->crdstm.pos, camera);
}

void	createview(t_list *objs, t_camera *camera, t_res *wincntr)
{
	t_list	*crsr;
	t_list	*camcrsr;
	int		i;

	crsr = objs;
	camcrsr = camera->camobjs;
	i = -1;
	while (crsr)
	{
		camera->objsinframe[++i] = objinframe(objcast(crsr), camera, wincntr);
		if (camera->objsinframe[i])
			definecamobj(objcast(crsr), objcast(camcrsr), camera);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

void	createcamobjs(t_list **camobjs, t_list *objs, t_bool **objsinframe)
{
	t_obj	*camobj;

	*objsinframe = malloc(sizeof(**objsinframe) * ft_lstsize(objs));
	while (objs)
	{
		ft_lstadd_front(camobjs, ft_lstnew(malloc(sizeof(t_obj))));
		camobj = objcast(*camobjs);
		camobj->dots.dotsnum = objcast(objs)->dots.dotsnum;
		camobj->dots.dots = objcast(objs)->dots.pos;
		camobj->dots.pos = malloc(sizeof(*camobj->dots.pos) * camobj->dots.dotsnum);
		camobj->dots.scale = 1;
		camobj->polys.poly = objcast(objs)->polys.poly;
		camobj->polys.txtr = objcast(objs)->polys.txtr;
		camobj->polys.polynum = objcast(objs)->polys.polynum;
		camobj->polys.polynorms = malloc(sizeof(*camobj->polys.polynorms) * camobj->polys.polynum);
		objs = objs->next;
	}
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
	info.mlx_ptr = mlx_init();
	wininit(&info.win, info.mlx_ptr, info.prog, *argv);
	ft_read_information(fd, &info);
	createcamobjs(&info.win.camera.camobjs, info.objects, &info.win.camera.objsinframe);
	createview(&info);
	backwards_ray_tracing(&info);
	return (0);
}

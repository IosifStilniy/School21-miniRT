/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_coloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 19:57:32 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/01 21:07:48 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	ft_get_sphere_cone_shadow(t_ray ray, t_polys *polys,
			t_cart *dots, float *closest_distance)
{
	int		i;
	t_cart	p[3];
	t_cart	phit;
	float	dist[2];

	i = -1;
	dist[0] = INFINITY;
	while (++i < polys->polynum)
	{
		ft_intersect_poly_plane(ray, &polys->poly[i].norm, &dots[polys->poly[i].vrtxs[0].dot], &dist[1]);
		if (dist[1] < dist[0])
		{
			p[0] = dots[polys->poly[i].vrtxs[0].dot];
			p[1] = dots[polys->poly[i].vrtxs[1].dot];
			p[2] = dots[polys->poly[i].vrtxs[2].dot];
			ft_multiply_vector(&ray.dir, dist[1] + 0.0001f, &phit);
			ft_summ_vectors(&phit, &ray.orig, &phit);
			if (ft_intersect_triangle(phit, &polys->poly[i], dots, NULL))
			{
				*closest_distance = dist[1];
				return ;
			}
		}
	}
}

static int	ft_is_in_shadow(t_cart *phit, t_list *object, t_cart *lightpos)
{
	t_ray	new_ray;
	t_cart	direction;
	float	distance_to_light;
	float	distance_to_object;
	t_obj	*current;

	distance_to_object = INFINITY;
	ft_substract_vectors(lightpos, phit, &direction);
	ft_get_vector_length(&direction, &distance_to_light);
	ft_cast_ray(&new_ray, &direction, phit);
	while (object)
	{
		current = (t_obj *)object->content;
		if (current->dots.dotsnum && ft_intersect_sphere(new_ray, current) < distance_to_object)
			ft_get_sphere_cone_shadow(new_ray, &current->polys, current->dots.pos, &distance_to_object);
		else if (!current->dots.dotsnum)
		{
			ft_multiply_vector(&new_ray.orig, 1 - 50 * __FLT_EPSILON__, &new_ray.orig);
			ft_intersect_plane(new_ray, NULL, NULL, current, &distance_to_object);
		}
		if (distance_to_object < distance_to_light)
			return (0);
		object = object->next;
	}
	return (1);
}

static int	ft_get_color_value(float color_koef[3], float l_color)
{
	float	color;

	color = color_koef[1] * l_color + color_koef[2] * l_color;
	return (color * 255);
}

/*
	n_vect[0]	-->	t_cart	n_view; // normal vector from phit towards camera
	n_vect[1]	-->	t_cart	n_light; // normal vector from phit towards light
	n_vect[2]	-->	t_cart	n_object; // normal vector of object at phit
	n_vect[3]	-->	t_cart	n_rlight; // reflected normal vector from phit
										towards light around object normal vector at phit 
*/

unsigned int	ft_shadowing(t_cart *phit, t_cart *object_norm,
			t_cart *object_color, t_info *info)
{
	t_list	*currl;
	int		i;
	t_cart	n_vect[4];
	t_cart	mltvec;
	t_cart	lk[2];
	float	ck[3];
	t_cart	color;

	if (phit->x == INFINITY || phit->y == INFINITY || phit->z == INFINITY)
		return (0x00000000);
	ck[0] = info->a_light.light_ratio;
	ft_multiply_vectors(&info->a_light.color, object_color, &lk[0]);
	i = -1;
	color.x = lk[0].x * ck[0] * 255;
	color.y = lk[0].y * ck[0] * 255;
	color.z = lk[0].z * ck[0] * 255;
	currl = info->lights;
	while (currl)
	{
		if (ft_is_in_shadow(phit, info->win.camera->objs, &info->win.camera->lightpos[++i]))
		{
			ft_inverse_vector(phit, &n_vect[0]);
			ft_get_vector_norm(&n_vect[0]);
			ft_substract_vectors(&info->win.camera->lightpos[i], phit, &n_vect[1]);
			ft_get_vector_norm(&n_vect[1]);
			n_vect[2] = *object_norm;
			ft_get_dot_product(&n_vect[1], &n_vect[2], &ck[1]);
			ft_multiply_vector(&n_vect[2], 2 * ck[1], &mltvec);
			ft_substract_vectors(&mltvec, &n_vect[1], &n_vect[3]);
			ft_get_vector_norm(&n_vect[3]);
			ck[1] = ft_max(ck[1], 0.0f) * ((t_light *)(currl->content))->light_ratio;
			ft_get_dot_product(&n_vect[3], &n_vect[0], &ck[2]);
			ck[2] = powf(ft_max(ck[2], 0.0f), 150.0f) * ((t_light *)(currl->content))->light_ratio;
			ft_multiply_vectors(&((t_light *)(currl->content))->color, object_color, &lk[1]);
			color.x += ft_get_color_value(ck, lk[1].x);
			color.y += ft_get_color_value(ck, lk[1].y);
			color.z += ft_get_color_value(ck, lk[1].z);
		}
		currl = currl->next;
	}
	if (color.x > 255.0f)
		color.x = 255.0f;
	if (color.y > 255.0f)
		color.y = 255.0f;
	if (color.z > 255.0f)
		color.z = 255.0f;
	return (ft_create_trgb(0, color.x, color.y, color.z));
}
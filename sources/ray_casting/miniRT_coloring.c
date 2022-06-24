/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_coloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 16:44:18 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/24 20:17:48 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

unsigned int	ft_create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

static void	ft_get_sphere_cone_shadow(t_ray ray, t_polys *polys,
			t_vrtx *dots, float *closest_distance)
{
	int		i;
	t_vrtx	p[3];
	t_cart	phit;
	float	dist[2];

	i = -1;
	dist[0] = INFINITY;
	while (++i < polys->polynum)
	{
		ft_intersect_plane(ray, polys->poly[i].norm,
			dots[polys->poly[i].dots[0]].dot, &dist[1]);
		if (dist[1] < dist[0])
		{
			p[0] = dots[polys->poly[i].dots[0]];
			p[1] = dots[polys->poly[i].dots[1]];
			p[2] = dots[polys->poly[i].dots[2]];
			phit = ft_summ_vectors(ft_multiply_vector(ray.dir, dist[1] + 0.0001f), ray.orig);
			if (ft_intersect_triangle(p, phit, dist, NULL, polys->poly[i].norm))
			{
				*closest_distance = dist[0];
				return ;
			}
		}
	}
}

static int	ft_is_in_shadow(t_cart phit, t_list *object, t_cart lightpos)
{
	t_ray	new_ray;
	t_cart	direction;
	float	distance_to_light;
	float	distance_to_object;
	t_obj	*current;

	distance_to_object = INFINITY;
	direction = ft_substract_vectors(lightpos, phit);
	distance_to_light = ft_get_vector_length(direction);
	ft_cast_ray(&new_ray, direction, phit);
	while (object)
	{
		current = (t_obj *)object->content;
		if (current->dots.dotsnum && ft_intersect_sphere(new_ray, current) < distance_to_object)
			ft_get_sphere_cone_shadow(new_ray, &current->polys, current->dots.pos, &distance_to_object);
		else if (!current->dots.dotsnum)
		{
			new_ray.orig = ft_multiply_vector(new_ray.orig, 1 - 50 * __FLT_EPSILON__);
			ft_intersect_plane(new_ray, current->crdstm.oz.vector, current->crdstm.pos, &distance_to_object);
		}
		if (distance_to_object < distance_to_light)
			return (0);
		object = object->next;
	}
	return (1);
}

static int	ft_get_color_value(float color_koef[3], float al_color,
			float l_color)
{
	float	color;

	color = color_koef[0] * al_color + color_koef[1]
		* l_color + color_koef[2] * l_color;
	if (color < 0)
		color = 0;
	else if (color > 1)
		color = 1;
	return (color * 255);
}

/*
	n_vect[0]	-->	t_cart	n_view; // normal vector from phit towards camera
	n_vect[1]	-->	t_cart	n_light; // normal vector from phit towards light
	n_vect[2]	-->	t_cart	n_object; // normal vector of object at phit
	n_vect[3]	-->	t_cart	n_rlight; // reflected normal vector from phit
										towards light around object normal vector at phit 
*/

unsigned int	ft_shadowing(t_cart phit, t_cart object_norm, t_cart object_color,
		t_info *info)
{
	t_cart	n_vect[4];
	t_cart	lk[2];
	float	ck[3];

	if (phit.x == INFINITY || phit.y == INFINITY || phit.z == INFINITY)
		return (0x00000000);
	ck[0] = info->a_light.light_ratio;
	lk[0] = ft_multiply_vectors(info->a_light.color, object_color);
	if (!ft_is_in_shadow(phit, info->win.camera.objs, info->win.camera.lightpos))
		return (ft_create_trgb(0, ck[0] * lk[0].x * 255,
				ck[0] * lk[0].y * 255, ck[0] * lk[0].z * 255));
	n_vect[0] = ft_inverse_vector(phit);
	n_vect[0] = ft_get_vector_norm(n_vect[0], ft_get_vector_length(n_vect[0]));
	n_vect[1] = ft_substract_vectors(info->win.camera.lightpos, phit);
	n_vect[1] = ft_get_vector_norm(n_vect[1], ft_get_vector_length(n_vect[1]));
	n_vect[2] = object_norm;
	n_vect[3] = ft_substract_vectors(ft_multiply_vector(n_vect[2], 2 * ft_get_dot_product(n_vect[1], n_vect[2])), n_vect[1]);
	n_vect[3] = ft_get_vector_norm(n_vect[3], ft_get_vector_length(n_vect[3]));
	ck[1] = ft_max(ft_get_dot_product(n_vect[1], n_vect[2]), 0.0f) * info->lights.light_ratio;
	ck[2] = powf(ft_max(ft_get_dot_product(n_vect[3], n_vect[0]), 0.0f), 1000.0f) * info->lights.light_ratio;
	lk[1] = ft_multiply_vectors(info->lights.color, object_color);
	return (ft_create_trgb(0, ft_get_color_value(ck, lk[0].x, lk[1].x), ft_get_color_value(ck, lk[0].y, lk[1].y), ft_get_color_value(ck, lk[0].z, lk[1].z)));
}
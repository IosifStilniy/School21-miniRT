/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_coloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 16:44:18 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/14 19:22:49 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	ft_create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

static int	ft_is_in_shadow(t_cart phit, t_list *object, t_cart lightpos)
{
	float	distance_to_object;
	float	distance_to_light;
	t_cart	new_ray;

	distance_to_object = INFINITY;
	new_ray = ft_substract_vectors(lightpos, phit);
	distance_to_light = ft_get_vector_length(new_ray);
	new_ray = ft_get_vector_norm(new_ray, ft_get_vector_length(new_ray));
	while (object)
	{
		if ((*(t_obj *)(object->content)).dots.dotsnum != 0)
		distance_to_object = ft_get_intersection_with_sphere(new_ray,
					ft_substract_vectors(phit,
						(t_cart){0.000001f, 0.000001f, 0.000001f}), object->content);
		// else
		// 	distance_to_object = ft_get_intersection_with_plane(new_ray,
		// 			ft_substract_vectors(phit,
		// 				(t_cart){0.000001f, 0.000001f, 0.000001f}), object->content);
		if (distance_to_object < distance_to_light)
			return (0);
		object = object->next;
	}
	return (1);
}

static	int	ft_get_color_value(int is_lighted, float color_koef[3],
	float al_color, float l_color)
{
	float	color;

	color = color_koef[0] * al_color + is_lighted
		* (color_koef[1] * l_color + color_koef[2] * l_color);
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

int	ft_find_light(t_cart phit, t_cart norm, t_cart color, t_info *info)
{
	t_cart	n_vect[4];
	t_cart	lk[2];
	float	ck[3];

	ck[0] = info->a_light.light_ratio;
	lk[0] = ft_multiply_vectors(info->a_light.color, color);
	if (!ft_is_in_shadow(phit, info->win.camera.objs, info->win.camera.lightpos))
		return (ft_create_trgb(0, ck[0] * lk[0].x * 255, ck[0] * lk[0].y * 255, ck[0] * lk[0].z * 255));
	n_vect[0] = ft_inverse_vector(phit);
	n_vect[0] = ft_get_vector_norm(n_vect[0], ft_get_vector_length(n_vect[0]));
	n_vect[1] = ft_substract_vectors(info->lights.pos, phit);
	n_vect[1] = ft_get_vector_norm(n_vect[1], ft_get_vector_length(n_vect[1]));
	n_vect[2] = ft_get_vector_norm(norm, ft_get_vector_length(norm));
	n_vect[3] = ft_substract_vectors(ft_multiply_vector(n_vect[2], 2 * ft_get_dot_product(n_vect[1], n_vect[2])), n_vect[1]);
	n_vect[3] = ft_get_vector_norm(n_vect[3], ft_get_vector_length(n_vect[3]));
	ck[1] = ft_max(ft_get_dot_product(n_vect[1], n_vect[2]), 0.0f) * info->lights.light_ratio;
	ck[2] = powf(ft_max(ft_get_dot_product(n_vect[3], n_vect[0]), 0.0f), 200.0f) * info->lights.light_ratio;
	lk[1] = ft_multiply_vectors(info->lights.color, color);
	return (ft_create_trgb(0, ft_get_color_value(1, ck, lk[0].x, lk[1].x), ft_get_color_value(1, ck, lk[0].y, lk[1].y), ft_get_color_value(1, ck, lk[0].z, lk[1].z)));
}
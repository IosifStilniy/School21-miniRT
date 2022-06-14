/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_intersections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 16:32:15 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/14 19:27:46 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_get_intersection_with_triangle(t_cart p[3],
	t_cart phit, t_cart norm_vector)
{
	t_cart	edge[3];
	t_cart	vec_c[3];

	edge[0] = ft_substract_vectors(p[1], p[0]);
	edge[1] = ft_substract_vectors(p[2], p[1]);
	edge[2] = ft_substract_vectors(p[0], p[2]);
	vec_c[0] = ft_substract_vectors(phit, p[0]);
	vec_c[1] = ft_substract_vectors(phit, p[1]);
	vec_c[2] = ft_substract_vectors(phit, p[2]);
	if (ft_get_dot_product(norm_vector, ft_get_cross_product(edge[0],
				vec_c[0])) > 0 && ft_get_dot_product(norm_vector,
			ft_get_cross_product(edge[1], vec_c[1])) > 0
		&& ft_get_dot_product(norm_vector,
			ft_get_cross_product(edge[2], vec_c[2])) > 0)
		return (1);
	return (0);
}

float	ft_get_intersection_with_poly(t_cart ray_dir,
	t_cart ray_orig, t_cart norm_vector, t_cart pos)
{
	float	ray_to_plane;
	float	division;
	t_cart	new_plane_pos;

	division = ft_get_dot_product(ray_dir, norm_vector);
	if (!division)
		return (INFINITY);
	new_plane_pos = ft_substract_vectors(ray_orig, pos);
	ray_to_plane = -ft_get_dot_product(new_plane_pos, norm_vector);
	if (!ray_to_plane)
		return (INFINITY);
	ray_to_plane /= division;
	if (ray_to_plane < 0.0001f)
		return (INFINITY);
	return (ray_to_plane);
}

float	ft_get_intersection_with_plane(t_cart ray_dir,
	t_cart ray_orig, t_obj *plane)
{
	float	ray_to_plane;
	float	division;
	t_cart	new_plane_pos;
	t_cart	norm_vector;

	norm_vector = plane->crdstm.oz.vector;
	division = ft_get_dot_product(ray_dir, norm_vector);
	if (!division)
		return (INFINITY);
	new_plane_pos = ft_substract_vectors(ray_orig, plane->crdstm.pos);
	ray_to_plane = -ft_get_dot_product(new_plane_pos, norm_vector);
	if (!ray_to_plane)
		return (INFINITY);
	ray_to_plane /= division;
	if (ray_to_plane < 0.001f)
		return (INFINITY);
	return (ray_to_plane);
}

float	ft_get_intersection_with_sphere(t_cart ray_dir,
	t_cart ray_orig, t_obj *sphere)
{
	float	radius_squared;
	float	ray_in_sphere_half;
	float	ray_to_intersect[2];
	float	center_to_ray_squared;
	float	ray_to_radius_perpendicular;

	ray_orig = ft_substract_vectors(sphere->crdstm.pos, ray_orig);
	ray_to_radius_perpendicular = ft_get_dot_product(ray_dir, ray_orig);
	if (ray_to_radius_perpendicular < 0)
		return (INFINITY);
	center_to_ray_squared = ft_get_dot_product(ray_orig, ray_orig)
		- ray_to_radius_perpendicular * ray_to_radius_perpendicular;
	radius_squared = sphere->outframe * sphere->outframe;
	if (center_to_ray_squared > radius_squared)
		return (INFINITY);
	ray_in_sphere_half = sqrtf(radius_squared - center_to_ray_squared);
	ray_to_intersect[0] = ray_to_radius_perpendicular - ray_in_sphere_half;
	ray_to_intersect[1] = ray_to_radius_perpendicular + ray_in_sphere_half;
	if (ray_to_intersect[0] > ray_to_intersect[1])
		ray_to_intersect[0] = ray_to_intersect[1];
	if (ray_to_intersect[0] < 0)
		return (INFINITY);
	return (ray_to_intersect[0]);
}

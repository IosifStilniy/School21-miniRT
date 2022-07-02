/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_intersections1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 16:32:15 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/02 13:53:36 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_hit_plane(t_ray ray, t_cart *nc,
			t_obj *object, float *closest_distance)
{
	float	ray_to_plane;
	float	division;
	t_cart	new_plane_pos;

	ft_dotprod(&ray.dir, &object->crdstm.oz.vector, &division);
	if (division < __FLT_EPSILON__ && division > -__FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	ft_subvects(&object->crdstm.pos, &ray.orig, &new_plane_pos);
	ft_dotprod(&new_plane_pos, &object->crdstm.oz.vector, &ray_to_plane);
	ray_to_plane /= division;
	if (ray_to_plane < __FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	*closest_distance = ray_to_plane;
	if (nc)
		nc[0] = object->crdstm.oz.vector;
	if (nc && !object->polys.txtr.img)
		nc[1] = *object->colrs;
}

void	ft_hit_pplane(t_ray ray, t_cart *norm_vector,
			t_cart *pos, float *closest_distance)
{
	float	ray_to_plane;
	float	division;
	t_cart	new_plane_pos;

	ft_dotprod(&ray.dir, norm_vector, &division);
	if (division < __FLT_EPSILON__ && division > -__FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	ft_subvects(pos, &ray.orig, &new_plane_pos);
	ft_dotprod(&new_plane_pos, norm_vector, &ray_to_plane);
	ray_to_plane /= division;
	if (ray_to_plane < __FLT_EPSILON__)
	{
		*closest_distance = INFINITY;
		return ;
	}
	*closest_distance = ray_to_plane;
}

float	ft_hit_sphere(t_ray ray, t_obj *sphere)
{
	float	radius_squared;
	float	ray_in_sphere_half;
	float	ray_to_intersect[2];
	float	center_to_ray_squared;
	float	ray_to_radius_perpendicular;

	ft_subvects(&sphere->crdstm.pos, &ray.orig, &ray.orig);
	ft_dotprod(&ray.dir, &ray.orig, &ray_to_radius_perpendicular);
	if (ray_to_radius_perpendicular < 0)
		return (INFINITY);
	ft_dotprod(&ray.orig, &ray.orig, &center_to_ray_squared);
	center_to_ray_squared -= ray_to_radius_perpendicular
		* ray_to_radius_perpendicular;
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

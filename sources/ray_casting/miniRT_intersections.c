// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   miniRT_intersections.c                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2022/06/05 16:32:15 by ncarob            #+#    #+#             */
// /*   Updated: 2022/06/25 18:56:27 by dcelsa           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minirt.h"

// void	ft_get_triangle_area(t_cart *dot1, t_cart *dot2, t_cart *dot3, float *area)
// {
// 	t_cart	dots[2];
// 	t_axis	axis;

// 	dots[0] = *dot2;
// 	dots[1] = *dot3;
// 	objtoobjpos(dot1, &dots[0]);
// 	objtoobjpos(dot1, &dots[1]);
// 	axisbuilder(&dots[0], &dots[1], &axis);
// 	*area = axis.length;
// }

// void	ft_get_baricentric_koefs(float k[3], t_vrtx p[3], t_cart *phit)
// {
// 	float	polyarea;

// 	ft_get_triangle_area(&p[0].dot, &p[1].dot, &p[2].dot, &polyarea);
// 	ft_get_triangle_area(phit, &p[1].dot, &p[2].dot, &k[0]);
// 	ft_get_triangle_area(phit, &p[0].dot, &p[2].dot, &k[1]);
// 	k[0] /= polyarea;
// 	k[1] /= polyarea;
// 	k[2] = 1.f - k[0] - k[1];
// }

// void	ft_get_interoplated_norm(t_vrtx p[3], t_cart *phit, t_cart *closest_norm)
// {
// 	float	k[3];
// 	t_cart	edge[3];
	
// 	ft_get_baricentric_koefs(k, p, phit);
// 	vectorsizing(k[0], &p[0].norm, &edge[0], NULL);
// 	vectorsizing(k[1], &p[1].norm, &edge[1], NULL);
// 	vectorsizing(k[2], &p[2].norm, &edge[2], NULL);
// 	cartbuilder(0, 0, 0, closest_norm);
// 	vectodot(closest_norm, &edge[0]);
// 	vectodot(closest_norm, &edge[1]);
// 	vectodot(closest_norm, &edge[2]);
// 	vectorsizing(1, closest_norm, closest_norm, NULL);
// }

// t_bool	ft_check_side_hit(t_cart *center, t_cart edge, t_cart phit, t_cart *norm)
// {
// 	t_axis	axis;

// 	objtoobjpos(center, &edge);
// 	objtoobjpos(center, &phit);
// 	axisbuilder(&edge, &phit, &axis);
// 	return (ft_get_dot_product(&axis.vector, norm) >= 0);
// }

// int	ft_intersect_triangle(t_vrtx p[3], t_cart *phit,
// 		float dist[3], t_cart *closest_norm, t_cart *norm)
// {
// 	t_bool	sides;

// 	sides = ft_check_side_hit(&p[0].dot, p[1].dot, *phit, norm);
// 	sides += ft_check_side_hit(&p[1].dot, p[2].dot, *phit, norm);
// 	sides += ft_check_side_hit(&p[2].dot, p[0].dot, *phit, norm);
// 	if (sides < 3)
// 		return (0);
// 	dist[0] = dist[1];
// 	if (!closest_norm || dist[0] < dist[2])
// 		return (0);
// 	ft_get_interoplated_norm(p, phit, closest_norm);
// 	return (1);
// }

// void	ft_intersect_plane(t_ray ray, t_cart *norm_vector,
// 			t_cart *pos, float *closest_distance)
// {
// 	float	ray_to_plane;
// 	float	division;
// 	t_cart	new_plane_pos;

// 	division = ft_get_dot_product(&ray.dir, norm_vector);
// 	if (division < __FLT_EPSILON__ && division > -__FLT_EPSILON__)
// 	{
// 		*closest_distance = INFINITY;
// 		return ;
// 	}
// 	new_plane_pos = ft_substract_vectors(pos, &ray.orig);
// 	ray_to_plane = ft_get_dot_product(&new_plane_pos, norm_vector) / division;
// 	if (ray_to_plane < __FLT_EPSILON__)
// 	{
// 		*closest_distance = INFINITY;
// 		return ;
// 	}
// 	*closest_distance = ray_to_plane;
// }

// float	ft_intersect_sphere(t_ray ray, t_obj *sphere)
// {
// 	float	radius_squared;
// 	float	ray_in_sphere_half;
// 	float	ray_to_intersect[2];
// 	float	center_to_ray_squared;
// 	float	ray_to_radius_perpendicular;

// 	ray.orig = ft_substract_vectors(&sphere->crdstm.pos, &ray.orig);
// 	ray_to_radius_perpendicular = ft_get_dot_product(&ray.dir, &ray.orig);
// 	if (ray_to_radius_perpendicular < 0)
// 		return (INFINITY);
// 	center_to_ray_squared = ft_get_dot_product(&ray.orig, &ray.orig)
// 		- ray_to_radius_perpendicular * ray_to_radius_perpendicular;
// 	radius_squared = sphere->outframe * sphere->outframe;
// 	if (center_to_ray_squared > radius_squared)
// 		return (INFINITY);
// 	ray_in_sphere_half = sqrtf(radius_squared - center_to_ray_squared);
// 	ray_to_intersect[0] = ray_to_radius_perpendicular - ray_in_sphere_half;
// 	ray_to_intersect[1] = ray_to_radius_perpendicular + ray_in_sphere_half;
// 	if (ray_to_intersect[0] > ray_to_intersect[1])
// 		ray_to_intersect[0] = ray_to_intersect[1];
// 	if (ray_to_intersect[0] < 0)
// 		return (INFINITY);
// 	return (ray_to_intersect[0]);
// }

// void	ft_intersect_polygon(t_ray ray, t_cart *closest_norm,
// 			t_obj *object, float *closest_distance)
// {
// 	int		i;
// 	t_vrtx	p[3];
// 	t_polys	polys;
// 	t_vrtx	*dots;
// 	t_cart	phit;
// 	t_cart	mltdir;
// 	float	dist[2];

// 	i = -1;
// 	dist[0] = INFINITY;
// 	polys = object->polys;
// 	dots = object->dots.pos;
// 	while (++i < polys.polynum)
// 	{
// 		ft_intersect_plane(ray, &polys.poly[i].norm,
// 			&dots[polys.poly[i].dots[0]].dot, &dist[1]);
// 		if (dist[1] < dist[0])
// 		{
// 			p[0] = dots[polys.poly[i].dots[0]];
// 			p[1] = dots[polys.poly[i].dots[1]];
// 			p[2] = dots[polys.poly[i].dots[2]];
// 			mltdir = ft_multiply_vector(&ray.dir, dist[1] + 0.0001f);
// 			phit = ft_summ_vectors(&mltdir, &ray.orig);
// 			ft_intersect_triangle(p, &phit, dist, closest_norm, &polys.poly[i].norm);
// 		}
// 	}
// 	*closest_distance = dist[0];
// }

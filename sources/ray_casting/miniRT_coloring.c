/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_coloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 19:57:32 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 17:59:13 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	ft_hit_sppoly(t_ray ray, t_polys *polys,
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
		ft_hit_pplane(ray, &polys->poly[i].norm,
			&dots[polys->poly[i].vrtxs[0].dot], &dist[1]);
		if (dist[1] < dist[0])
		{
			p[0] = dots[polys->poly[i].vrtxs[0].dot];
			p[1] = dots[polys->poly[i].vrtxs[1].dot];
			p[2] = dots[polys->poly[i].vrtxs[2].dot];
			ft_multvect(&ray.dir, dist[1] + 0.0001f, &phit);
			ft_summvects(&phit, &ray.orig, &phit);
			if (ft_hit_triangle(phit, &polys->poly[i], dots, NULL))
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
	float	lightdist;
	float	objdist;
	t_obj	*curr;

	objdist = INFINITY;
	ft_subvects(lightpos, phit, &direction);
	ft_vectlen(&direction, &lightdist);
	ft_cast_ray(&new_ray, &direction, phit);
	while (object)
	{
		curr = (t_obj *)object->content;
		if (curr->dots.dotsnum && ft_hit_sphere(new_ray, curr) < objdist)
			ft_hit_sppoly(new_ray, &curr->polys, curr->dots.pos, &objdist);
		else if (!curr->dots.dotsnum)
		{
			ft_multvect(&new_ray.orig, 1 - 50 * __FLT_EPSILON__, &new_ray.orig);
			ft_hit_plane(new_ray, NULL, curr, &objdist);
		}
		if (objdist < lightdist)
			return (0);
		object = object->next;
	}
	return (1);
}

static void	ft_get_color_value(float ck[3], t_cart *al_colr,
			t_cart *dl_colr, t_cart *f_colr)
{
	if (al_colr)
	{
		f_colr->x = ck[0] * al_colr->x * 255.0f;
		f_colr->y = ck[0] * al_colr->y * 255.0f;
		f_colr->z = ck[0] * al_colr->z * 255.0f;
	}
	if (dl_colr)
	{
		f_colr->x += (ck[1] * dl_colr->x + ck[2] * dl_colr->x) * 255.0f;
		f_colr->y += (ck[1] * dl_colr->y + ck[2] * dl_colr->y) * 255.0f;
		f_colr->z += (ck[1] * dl_colr->z + ck[2] * dl_colr->z) * 255.0f;
	}
	if (f_colr->x > 255.0f)
		f_colr->x = 255.0f;
	if (f_colr->y > 255.0f)
		f_colr->y = 255.0f;
	if (f_colr->z > 255.0f)
		f_colr->z = 255.0f;
}

/*
	carts[0]	-->	t_cart	n_view; // normal vector from phit towards camera
	carts[1]	-->	t_cart	n_light; // normal vector from phit towards light
	carts[2]	-->	t_cart	n_object; // normal vector of object at phit
	carts[3]	-->	t_cart	n_rlight; // reflected normal vector from phit	
	towards light around object normal vector at phit 
	carts[4]	--> t_cart	phit // used for and as tmp in between calculations
	carts[5]	-->	t_cart	light_coefficient[0] // for ambient lightning
	carts[6]	-->	t_cart	light_coefficient[1] // for df & sp lightning
	carts[7]	-->	t_cart	color_object // objects color values
	carts[8]	-->	t_cart	color	// final r g b values
*/

void	ft_iter_lights(t_cart *carts, float *ck,
			t_light *light, t_cart *lightpos)
{
	ft_invvect(&carts[4], &carts[0]);
	ft_vectnorm(&carts[0]);
	ft_subvects(lightpos, &carts[4], &carts[1]);
	ft_vectnorm(&carts[1]);
	ft_dotprod(&carts[1], &carts[2], &ck[1]);
	ft_multvect(&carts[2], 2 * ck[1], &carts[4]);
	ft_subvects(&carts[4], &carts[1], &carts[3]);
	ft_vectnorm(&carts[3]);
	ck[1] = ft_max(ck[1], 0.0f) * light->light_ratio;
	if (!ck[1])
	{
		ft_invvect(&carts[2], &carts[2]);
		ft_dotprod(&carts[1], &carts[2], &ck[1]);
	}
	ft_dotprod(&carts[3], &carts[0], &ck[2]);
	ck[2] = powf(ft_max(ck[2], 0.0f), 150.0f) * light->light_ratio;
	ft_multvects(&light->color, &carts[7], &carts[6]);
	ft_get_color_value(ck, NULL, &carts[6], &carts[8]);
}

void	ft_shadowing(unsigned int *color, t_cart *phit,
			t_cart *nc, t_info *info)
{
	int		i;
	t_list	*curr;
	float	ck[3];
	t_cart	carts[9];

	*color = 0;
	if (phit->x == INFINITY || phit->y == INFINITY || phit->z == INFINITY)
		return ;
	ck[0] = info->a_light.light_ratio;
	ft_multvects(&info->a_light.color, &nc[1], &carts[5]);
	ft_get_color_value(ck, &carts[5], NULL, &carts[8]);
	i = -1;
	curr = info->lights;
	carts[2] = nc[0];
	carts[7] = nc[1];
	carts[4] = *phit;
	while (curr)
	{
		if (ft_is_in_shadow(phit, info->win.camera->objs,
				&info->win.camera->lightpos[++i]))
			ft_iter_lights(carts, ck, curr->content,
				&info->win.camera->lightpos[i]);
		curr = curr->next;
	}
	*color = ft_create_trgb(0, carts[8].x, carts[8].y, carts[8].z);
}

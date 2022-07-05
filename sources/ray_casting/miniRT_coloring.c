/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_coloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 19:57:32 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 18:53:26 by ncarob           ###   ########.fr       */
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

static void	ft_get_color_value(t_cart *ck, t_cart *al_colr,
			t_cart *dl_colr, t_cart *f_colr)
{
	if (al_colr)
	{
		f_colr->x = ck->x * al_colr->x * 255.0f;
		f_colr->y = ck->x * al_colr->y * 255.0f;
		f_colr->z = ck->x * al_colr->z * 255.0f;
	}
	if (dl_colr)
	{
		f_colr->x += (ck->y * dl_colr->x + ck->z * dl_colr->x) * 255.0f;
		f_colr->y += (ck->y * dl_colr->y + ck->z * dl_colr->y) * 255.0f;
		f_colr->z += (ck->y * dl_colr->z + ck->z * dl_colr->z) * 255.0f;
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
	ncd[0]		-->	t_cart	n_object; // normal vector of object at phit
	carts[2]	-->	t_cart	n_rlight; // reflected normal vector from phit	
					towards light around object normal vector at phit 
	carts[3]	--> t_cart	phit // used for and as tmp in between calculations
	carts[4]	-->	t_cart	light_coefficient[0] // for ambient lightning
	carts[5]	-->	t_cart	light_coefficient[1] // for df & sp lightning
	ncd[1]		-->	t_cart	color_object // objects color values
	carts[6]	-->	t_cart	color	// final r g b values
	carts[7]	--> t_cart	ck[3] --> 3 floats ambient diffuse spec intensity
*/

void	ft_iter_lights(t_cart *carts, t_cart *ncd,
			t_light *light, t_cart *lightpos)
{
	ft_invvect(&carts[3], &carts[0]);
	ft_vectnorm(&carts[0]);
	ft_subvects(lightpos, &carts[3], &carts[1]);
	ft_vectnorm(&carts[1]);
	ft_dotprod(&carts[1], &ncd[0], &carts[7].y);
	ft_multvect(&ncd[0], 2 * carts[7].y, &carts[3]);
	ft_subvects(&carts[3], &carts[1], &carts[2]);
	ft_vectnorm(&carts[2]);
	carts[7].y = ft_max(carts[7].y, 0.0f) * light->light_ratio;
	if (!carts[7].y && !ncd[2].x)
	{
		ft_invvect(&ncd[0], &ncd[0]);
		ft_dotprod(&carts[1], &ncd[0], &carts[7].y);
	}
	ft_dotprod(&carts[2], &carts[0], &carts[7].z);
	carts[7].z = powf(ft_max(carts[7].z, 0.0f), 150.0f) * light->light_ratio;
	ft_multvects(&light->color, &ncd[1], &carts[5]);
	ft_get_color_value(&carts[7], NULL, &carts[5], &carts[6]);
}

void	ft_shadowing(unsigned int *color, t_cart *phit,
			t_cart *ncd, t_info *info)
{
	int		i;
	t_list	*curr;
	t_cart	carts[8];

	*color = 0;
	if (phit->x == INFINITY || phit->y == INFINITY || phit->z == INFINITY)
		return ;
	carts[7].x = info->a_light.light_ratio;
	ft_multvects(&info->a_light.color, &ncd[1], &carts[4]);
	ft_get_color_value(&carts[7], &carts[4], NULL, &carts[6]);
	i = -1;
	curr = info->lights;
	carts[3] = *phit;
	while (curr)
	{
		if (ft_is_in_shadow(phit, info->win.camera->objs,
				&info->win.camera->lightpos[++i]))
			ft_iter_lights(carts, ncd, curr->content,
				&info->win.camera->lightpos[i]);
		curr = curr->next;
	}
	*color = ft_create_trgb(0, carts[6].x, carts[6].y, carts[6].z);
}

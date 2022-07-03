/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_hitions2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 16:32:15 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/02 14:08:49 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	ft_check_if_inside(t_cart *phit, t_poly *poly,
			t_cart *dots, float k[3])
{
	t_cart	edge;
	t_cart	vec_c;
	t_cart	res[3];
	float	res1[3];

	ft_subvects(&dots[poly->vrtxs[0].dot], &dots[poly->vrtxs[1].dot], &edge);
	ft_subvects(phit, &dots[poly->vrtxs[0].dot], &vec_c);
	ft_crossprod(&edge, &vec_c, &res[0]);
	ft_dotprod(&poly->norm, &res[0], &res1[0]);
	ft_subvects(&dots[poly->vrtxs[1].dot], &dots[poly->vrtxs[2].dot], &edge);
	ft_subvects(phit, &dots[poly->vrtxs[1].dot], &vec_c);
	ft_crossprod(&edge, &vec_c, &res[1]);
	ft_dotprod(&poly->norm, &res[1], &res1[1]);
	ft_subvects(&dots[poly->vrtxs[2].dot], &dots[poly->vrtxs[0].dot], &edge);
	ft_subvects(phit, &dots[poly->vrtxs[2].dot], &vec_c);
	ft_crossprod(&edge, &vec_c, &res[2]);
	ft_dotprod(&poly->norm, &res[2], &res1[2]);
	if (res1[0] > 0 || res1[1] > 0 || res1[2] > 0)
		return (0);
	if (k)
	{
		ft_vectlen(&res[1], &k[0]);
		ft_vectlen(&res[0], &k[1]);
	}
	return (1);
}

int	ft_hit_triangle(t_cart phit, t_poly *poly, t_cart *dots, float k[3])
{
	t_cart	edge;
	t_cart	vec_c;
	t_cart	res;
	float	denom;

	ft_subvects(&dots[poly->vrtxs[1].dot],
		&dots[poly->vrtxs[0].dot], &edge);
	ft_subvects(&dots[poly->vrtxs[2].dot],
		&dots[poly->vrtxs[0].dot], &vec_c);
	ft_crossprod(&edge, &vec_c, &res);
	ft_vectlen(&res, &denom);
	if (!ft_check_if_inside(&phit, poly, dots, k))
		return (0);
	if (!k)
		return (1);
	k[0] /= denom;
	k[1] /= denom;
	k[2] = 1.0f - k[1] - k[0];
	return (1);
}

void	ft_hit_poly2(t_obj *obj, float *k, int *i, t_cart *nc)
{
	t_cart	phit;
	t_poly	*poly;

	poly = &obj->polys.poly[*i];
	ft_multvect(&poly->vrtxs[0].norm, k[0], &nc[0]);
	ft_multvect(&poly->vrtxs[1].norm, k[2], &phit);
	ft_summvects(&phit, &nc[0], &nc[0]);
	ft_multvect(&poly->vrtxs[2].norm, k[1], &phit);
	ft_summvects(&phit, &nc[0], &nc[0]);
	ft_vectnorm(&nc[0]);
	if (!obj->polys.txtr.img)
		nc[1] = *obj->colrs;
	else
	{
		ft_multvect(&poly->vrtxs[0].uv, k[0], &nc[1]);
		ft_multvect(&poly->vrtxs[1].uv, k[2], &phit);
		ft_summvects(&phit, &nc[1], &nc[1]);
		ft_multvect(&poly->vrtxs[2].uv, k[1], &phit);
		ft_summvects(&phit, &nc[1], &nc[1]);
		if (obj->polys.checkerboard)
			ft_checker_poly(&nc[1]);
		else
			my_mlx_get_pixel(poly->txtr, &nc[1]);
	}
}

void	ft_hit_poly(t_ray ray, t_cart *nc, t_obj *object, float *c_dist)
{
	int		i;
	t_cart	phit;
	float	k[3];
	float	dist[2];

	i = -1;
	dist[0] = INFINITY;
	while (++i < object->polys.polynum)
	{
		ft_hit_pplane(ray, &object->polys.poly[i].norm,
			&object->dots.pos[object->polys.poly[i].vrtxs[0].dot], &dist[1]);
		if (dist[1] < dist[0])
		{
			ft_multvect(&ray.dir, dist[1] + 0.0001f, &phit);
			ft_summvects(&phit, &ray.orig, &phit);
			if (ft_hit_triangle(phit, &object->polys.poly[i],
					object->dots.pos, k))
			{
				dist[0] = dist[1];
				ft_hit_poly2(object, k, &i, nc);
			}
		}
	}
	*c_dist = dist[0];
}

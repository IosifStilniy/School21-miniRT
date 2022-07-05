/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conebuilder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:07:37 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 15:08:05 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	vrtxnorm(t_cart *dots, t_vrtx *vrtx)
{
	t_cart	norm;
	t_cart	dot;
	float	scalar;

	normbuilder(&dots[vrtx->dot], &dots[0], &dots[1], &norm);
	dot = dots[1];
	objtoobjpos(&dots[vrtx->dot], &dot);
	normbuilder(NULL, &norm, &dot, &vrtx->srcnorm);
	ft_dotprod(&vrtx->srcnorm, &dots[vrtx->dot], &scalar);
	if (scalar < 0)
		negativevector(&vrtx->srcnorm);
}

static void	defineconevrtxs(t_cart *dots, t_poly *polys,
			int polynum, t_data *txtr)
{
	int		i;

	i = -1;
	while (++i < RNDSGMNTS)
		buttvrtxing(&polys[i], txtr);
	while (++i < polynum)
	{
		polys[i].txtr = txtr;
		polys[i].interpolate = TRUE;
		vrtxnorm(dots, &polys[i].vrtxs[1]);
		vrtxnorm(dots, &polys[i].vrtxs[2]);
	}
}

static void	conemapping(t_cart *dots, t_poly *polys, int polynum)
{
	int	i;

	i = -1;
	while (++i < polynum)
	{
		polys[i].vrtxs[0].uv.y = (dots[polys[i].vrtxs[0].dot].z < 0);
		polys[i].vrtxs[0].uv.x = (dots[polys[i].vrtxs[0].dot].z < 0);
		polys[i].vrtxs[1].uv.y = 0.f;
		polys[i].vrtxs[1].uv.x = (float)((polys[i].vrtxs[1].dot - 2)
				% RNDSGMNTS) / RNDSGMNTS;
		polys[i].vrtxs[2].uv.y = 0.f;
		polys[i].vrtxs[2].uv.x = (float)((polys[i].vrtxs[1].dot - 2)
				% RNDSGMNTS) / RNDSGMNTS;
		repairbackpatch(polys[i].vrtxs);
	}
}

float	conebuilder(t_dots *dots, t_polys *polys, float radius, float height)
{
	int		i;

	dots->dotsnum = RNDSGMNTS + 1;
	polys->polynum = RNDSGMNTS * 2;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	cartbuilder(0, 0, -height / 3, &dots->dots[0]);
	cartbuilder(0, 0, 2 * height / 3, &dots->dots[1]);
	circledotsfiller(&dots->dots[2], radius, NULL, FALSE);
	i = -1;
	while (++i < RNDSGMNTS)
		dots->dots[2 + i].z = -height / 3;
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	buttsurf(0, 2, polys->poly, dots->dots);
	buttsurf(1, 2, &polys->poly[RNDSGMNTS], dots->dots);
	defineconevrtxs(dots->dots, polys->poly, polys->polynum, &polys->txtr);
	conemapping(dots->dots, polys->poly, polys->polynum);
	return (vectorlength(&dots->dots[1]));
}

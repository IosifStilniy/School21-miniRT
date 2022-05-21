/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/21 17:57:46 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	quartrot(t_cart *pos, t_axis *axis)
{	
	double	mlts[3][3];
	t_cart	axsr;

	axsr.x = sinf(axis->ang / 2) * axis->vector.x;
	axsr.y = sinf(axis->ang / 2) * axis->vector.y;
	axsr.z = sinf(axis->ang / 2) * axis->vector.z;
	mlts[0][0] = (1 - 2 * (powf(axsr.y, 2) + powf(axsr.z, 2))) * pos->x;
	mlts[0][1] = (2 * (axsr.x * axsr.y - axsr.z * cosf(axis->ang / 2))) * pos->y;
	mlts[0][2] = (2 * (axsr.x * axsr.z + axsr.y * cosf(axis->ang / 2))) * pos->z;
	mlts[1][0] = (2 * (axsr.x * axsr.y + axsr.z * cosf(axis->ang / 2))) * pos->x;
	mlts[1][1] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.z, 2))) * pos->y;
	mlts[1][2] = (2 * (axsr.y * axsr.z - axsr.x * cosf(axis->ang / 2))) * pos->z;
	mlts[2][0] = (2 * (axsr.x * axsr.z - axsr.y * cosf(axis->ang / 2))) * pos->x;
	mlts[2][1] = (2 * (axsr.y * axsr.z + axsr.x * cosf(axis->ang / 2))) * pos->y;
	mlts[2][2] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.y, 2))) * pos->z;
	pos->x = mlts[0][0] + mlts[0][1] + mlts[0][2];
	pos->y = mlts[1][0] + mlts[1][1] + mlts[1][2];
	pos->z = mlts[2][0] + mlts[2][1] + mlts[2][2];
}

void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst, int scale, t_cart *k)
{
	dst->x = src->x * scale * k->x;
	dst->y = src->y * scale * k->y;
	dst->z = src->z * scale * k->z;
}

void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm)
{
	int		i;
	t_cart	k;

	k.x = crdstm->ox.vector.x + crdstm->oy.vector.x + crdstm->oz.vector.x;
	k.y = crdstm->ox.vector.y + crdstm->oy.vector.y + crdstm->oz.vector.y;
	k.z = crdstm->ox.vector.z + crdstm->oy.vector.z + crdstm->oz.vector.z;
	i = -1;
	while (++i < dots->dotsnum)
	{
		dotcrdstmtrnsltn(&dots->dots[i].dot, &dots->pos[i].dot, dots->scale, &k);
		dotcrdstmtrnsltn(&dots->dots[i].norm, &dots->pos[i].norm, 1, &k);
	}
	i = -1;
	while (++i < polys->polynum)
		dotcrdstmtrnsltn(&polys->poly[i].srcnorm, &polys->poly[i].norm, 1, &k);
}

void	flatanglehandler(t_rot *rot, t_cart *ref)
{
	float	ang;

	axisbuilder(rot->start, rot->end, &rot->axis);
	ang = rot->axis.ang;
	if (!comparef(rot->axis.length, 0, 0.0001) && !comparef(rot->axis.ang, M_PI, 0.001))
		;
	else if (!ref)
	{
		vectorbuilder(1, 0, 0, &rot->axis);
		if (!comparef(rot->end->x, 0, 0.001) || !comparef(rot->end->z, 0, 0.001))
			vectorbuilder(0, 1, 0, &rot->axis);
	}
	else if (ref != rot->start)
		axisbuilder(ref, rot->end, &rot->axis);
	rot->axis.ang = ang;
}

void	objtoobjaxis(t_crdstm *src, t_crdstm *dst, t_rot *rot)
{
	t_crdstm	world;
	t_axis		zaxis;
	
	vectorbuilder(1, 0, 0, &world.ox);
	vectorbuilder(0, 1, 0, &world.oy);
	vectorbuilder(0, 0, 1, &world.oz);
	if (!src)
		src = &world;
	if (!dst)
		dst = &world;
	crdstmrot(src, rot, &src->oz.vector, &dst->oz.vector);
	zaxis = rot->axis;
	crdstmrot(src, rot, &src->ox.vector, &dst->ox.vector);
	rot->xyaxis = rot->axis;
	rot->axis = zaxis;
}

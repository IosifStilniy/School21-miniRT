/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/17 22:03:02 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	quartrot(t_cart *pos, t_axis *axis)
{	
	float	mlts[3][3];
	float	halfang;
	float	real;
	t_cart	axsr;

	halfang = axis->ang / 2;
	real = cosf(halfang);
	axsr.x = sinf(halfang) * axis->vector.x;
	axsr.y = sinf(halfang) * axis->vector.y;
	axsr.z = sinf(halfang) * axis->vector.z;
	mlts[0][0] = (1 - 2 * (powf(axsr.y, 2) + powf(axsr.z, 2))) * pos->x;
	mlts[0][1] = (2 * (axsr.x * axsr.y - axsr.z * real)) * pos->y;
	mlts[0][2] = (2 * (axsr.x * axsr.z + axsr.y * real)) * pos->z;
	mlts[1][0] = (2 * (axsr.x * axsr.y + axsr.z * real)) * pos->x;
	mlts[1][1] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.z, 2))) * pos->y;
	mlts[1][2] = (2 * (axsr.y * axsr.z - axsr.x * real)) * pos->z;
	mlts[2][0] = (2 * (axsr.x * axsr.z - axsr.y * real)) * pos->x;
	mlts[2][1] = (2 * (axsr.y * axsr.z + axsr.x * real)) * pos->y;
	mlts[2][2] = (1 - 2 * (powf(axsr.x, 2) + powf(axsr.y, 2))) * pos->z;
	pos->x = mlts[0][0] + mlts[0][1] + mlts[0][2];
	pos->y = mlts[1][0] + mlts[1][1] + mlts[1][2];
	pos->z = mlts[2][0] + mlts[2][1] + mlts[2][2];
}

void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst, int scale, t_crdstm *crdstm)
{
	dst->x = src->x * crdstm->ox.vector.x + src->y * crdstm->oy.vector.x + src->z * crdstm->oz.vector.x;
	dst->y = src->x * crdstm->ox.vector.y + src->y * crdstm->oy.vector.y + src->z * crdstm->oz.vector.y;
	dst->z = src->x * crdstm->ox.vector.z + src->y * crdstm->oy.vector.z + src->z * crdstm->oz.vector.z;
	if (comparef(scale, 1, 0.001))
		return ;
	dst->x *= scale;
	dst->y *= scale;
	dst->z *= scale;
}

void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm)
{
	int		i;

	i = -1;
	while (++i < dots->dotsnum)
	{
		dotcrdstmtrnsltn(&dots->dots[i].dot, &dots->pos[i].dot, dots->scale, crdstm);
		dots->pos[i].dot.x += crdstm->pos.x;
		dots->pos[i].dot.y += crdstm->pos.y;
		dots->pos[i].dot.z += crdstm->pos.z;
		dotcrdstmtrnsltn(&dots->dots[i].norm, &dots->pos[i].norm, 1, crdstm);
	}
	i = -1;
	while (++i < polys->polynum)
		dotcrdstmtrnsltn(&polys->poly[i].srcnorm, &polys->poly[i].norm, 1, crdstm);
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
	axisbuilder(&src->oz.vector, &dst->oz.vector, &zaxis);
	if (comparef(zaxis.ang, M_PI, 0.0001))
		cartcopy(&src->ox.vector, &zaxis.vector, 1);
	crdstmrotbyaxis(src, &zaxis, NULL);
	rot->axis = zaxis;
	negativevector(&rot->axis.vector);
	axisbuilder(&src->ox.vector, &dst->ox.vector, &zaxis);
	if (comparef(zaxis.ang, M_PI, 0.0001))
		cartcopy(&src->oz.vector, &zaxis.vector, 1);
	rot->xyaxis = zaxis;
	negativevector(&rot->xyaxis.vector);
}

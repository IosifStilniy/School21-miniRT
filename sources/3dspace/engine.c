/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/11 22:05:35 by dcelsa           ###   ########.fr       */
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

void	engine(t_dots *dots, t_polys *polys, t_axis *axis)
{
	int		i;

	i = -1;
	while (++i < dots->dotsnum)
	{
		dots->pos[i].x = dots->dots[i].x * dots->scale;
		dots->pos[i].y = dots->dots[i].y * dots->scale;
		dots->pos[i].z = dots->dots[i].z * dots->scale;
		quartrot(&dots->pos[i], axis);
	}
	i = -1;
	if (!polys->polynorms)
		while (++i < polys->polynum)
			quartrot(&polys->poly[i].norm.vector, axis);
	else
		while (++i < polys->polynum)
			quartrot(&polys->polynorms[i].vector, axis);
}

void	crdstmrot(t_crdstm *crdstm, t_rot *rot, t_axis *start, t_axis *end)
{
	float	ang;
	t_axis	ref;

	rot->start = start;
	rot->end = end;
	flatanglehandler(rot, NULL);
	if (&crdstm->oz != start)
	{
		ang = rot->axis.ang;
		axisbuilder(&crdstm->oz, &rot->axis, &ref);
		rot->axis = crdstm->oz;
		if (comparef(ref.ang, M_PI, 0.001))
			negativevector(&rot->axis.vector);
		rot->axis.ang = ang;
	}
	quartrot(&crdstm->oz.vector, &rot->axis);
	quartrot(&crdstm->oy.vector, &rot->axis);
	quartrot(&crdstm->ox.vector, &rot->axis);
}

void	rotnpersp(t_cart *pos, t_axis *zrot, t_axis *xyrot, float focus)
{
	quartrot(pos, &zrot);
	quartrot(pos, &xyrot);
	if (comparef(pos->z, 0, 0.001) || comparef(focus, 0, 0.001) || focus == INFINITY)
		return ;
	pos->x *= focus / pos->z;
	pos->y *= focus / pos->z;
}

void	wrldtocamcoords(t_dots *dots, t_polys *polys, t_cart *crdstm, t_camera *camera)
{
	t_crdstm	world;
	t_axis		zrot;
	t_axis		xyrot;
	int			i;
	
	vectorbuilder(0, 0, 1, &world.oz);
	crdstmdefiner(&world);
	crdstmrot(&world, camera->rot, &world.oz, &camera->crdstm.oz);
	zrot = camera->rot->axis;
	crdstmrot(&world, camera->rot, &world.ox, &camera->crdstm.ox);
	xyrot = camera->rot->axis;
	i = -1;
	while (++i < dots->dotsnum)
	{
		dots->pos[i].x = dots->dots[i].x + crdstm->x - camera->pos.x;
		dots->pos[i].y = dots->dots[i].y + crdstm->y - camera->pos.y;
		dots->pos[i].z = dots->dots[i].z + crdstm->z - camera->pos.z;
		quartrot(dots->pos + i, &zrot);
		quartrot(dots->pos + i, &xyrot);
	}
	i = -1;
	while (++i < polys->polynum)
	{
		quartrot(&polys->polynorms[i].vector, &zrot);
		quartrot(&polys->polynorms[i].vector, &xyrot);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/10 22:37:15 by dcelsa           ###   ########.fr       */
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
	
	rot->start = start;
	rot->end = end;
	flatanglehandler(rot, NULL);
	if (&crdstm->oz != start)
	{
		ang = rot->axis.ang;
		rot->axis = crdstm->oz;
		rot->axis.ang = ang;
	}
	quartrot(&crdstm->oz.vector, &rot->axis);
	quartrot(&crdstm->oy.vector, &rot->axis);
	quartrot(&crdstm->ox.vector, &rot->axis);
}

void	translateobj(t_obj *obj, t_cart *shift)
{
	obj->crdstm.pos.x += shift->x;
	obj->crdstm.pos.y += shift->y;
	obj->crdstm.pos.z += shift->z;
}

void	computeworldcoords(t_obj *obj, t_camera *camera)
{
	t_crdstm	world;
	t_axis		zrot;
	t_axis		xyrot;
	int			i;
	
	vectorbuilder(1, 0, 0, &world.ox);
	vectorbuilder(0, 1, 0, &world.oy);
	vectorbuilder(0, 0, 1, &world.oz);
	crdstmrot(&world, camera->rot, &world.oz, &camera->crdstm.oz);
	zrot = camera->rot->axis;
	crdstmrot(&world, camera->rot, &world.ox, &camera->crdstm.ox);
	xyrot = camera->rot->axis;
	i = -1;
	while (++i < obj->dots.dotsnum)
	{
		obj->dots.pos[i].x += obj->crdstm.pos.x - camera->pos.x;
		obj->dots.pos[i].y += obj->crdstm.pos.y - camera->pos.y;
		obj->dots.pos[i].z += obj->crdstm.pos.z - camera->pos.z;
		quartrot(&obj->dots.pos[i], &zrot);
		quartrot(&obj->dots.pos[i], &xyrot);
	}
	i = -1;
	while (++i < obj->polys.polynum)
	{
		quartrot(&obj->polys.polynorms[i].vector, &zrot);
		quartrot(&obj->polys.polynorms[i].vector, &xyrot);
	}
}

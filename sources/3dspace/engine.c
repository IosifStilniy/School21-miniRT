/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/09 16:12:18 by dcelsa           ###   ########.fr       */
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
	while (++i < polys->polynum)
		quartrot(&polys->poly[i].norm.vector, axis);
}

void	crdstmrot(t_crdstm *crdstm, t_rot *rot, t_axis *end)
{
	rot->start = &crdstm->oz;
	rot->end = end;
	flatanglehandler(rot, NULL);
	quartrot(&crdstm->oz.vector, rot->axis);
	quartrot(&crdstm->oy.vector, rot->axis);
	quartrot(&crdstm->ox.vector, rot->axis);
}

void	objrot(t_obj *obj, t_rot *rot, t_axis *end)
{
	int	i;

	vectorbuilder(0, 0, 1, rot->start);
	rot->end = end;
	flatanglehandler(rot, &obj->crdstm.oz);
	engine(&obj->dots, &obj->polys, end);
	crdstmrot(&obj->crdstm, rot, end);
}

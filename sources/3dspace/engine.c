/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/07 14:42:33 by dcelsa           ###   ########.fr       */
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

void	engine(t_win *win, t_dot *dots)
{
	int		i;
	int		j;

	quartrot(&dots->norm.vector, &win->view.axis);
	i = -1;
	while (++i < dots->rows)
	{
		j = -1;
		while (++j < dots->cols)
		{
			quartrot(&dots->dots[i][j], &win->view.axis);
			dots->pos[i][j].x = dots->dots[i][j].x * win->view.scale.cur;
			dots->pos[i][j].y = dots->dots[i][j].y * win->view.scale.cur;
			dots->pos[i][j].z = dots->dots[i][j].z * win->view.scale.cur;
			if (win->view.perspective)
			{
				dots->pos[i][j].x *= win->view.focus
					/ (win->view.dstnc + dots->pos[i][j].z);
				dots->pos[i][j].y *= win->view.focus
					/ (win->view.dstnc + dots->pos[i][j].z);
			}
		}
	}
	win->view.axis.ang = 0;
}

void	axisbuilder(t_axis *v1, t_axis *v2, t_axis *axis)
{
	float	scalar;

	axis->vector.x = v1->vector.y * v2->vector.z - v1->vector.z * v2->vector.y;
	axis->vector.y = v1->vector.z * v2->vector.x - v1->vector.x * v2->vector.z;
	axis->vector.z = v1->vector.x * v2->vector.y - v1->vector.y * v2->vector.x;
	axis->length = sqrtf(powf(axis->vector.x, 2) + powf(axis->vector.y, 2)
			+ powf(axis->vector.z, 2));
	scalar = axis->length;
	scalar /= scalar * (scalar >= 1) + (scalar < 1);
	axis->ang = asinf(scalar);
	scalar = v1->vector.x * v2->vector.x + v1->vector.y * v2->vector.y
		+ v1->vector.z * v2->vector.z;
	if (scalar < 0)
		axis->ang = M_PI - axis->ang;
	if (!axis->length)
		return ;
	axis->vector.x /= axis->length;
	axis->vector.y /= axis->length;
	axis->vector.z /= axis->length;
}

void	flatanglehandler(t_rot *rot, t_axis *ref)
{
	float	ang;

	axisbuilder(rot->start, rot->end, rot->axis);
	ang = rot->axis->ang;
	if (!comparef(rot->axis->length, 0, 0.0001) && !comparef(rot->axis->ang, M_PI, 0.001))
		;
	else if (!ref)
	{
		vectorbuilder(1, 0, 0, rot->axis);
		if (!comparef(rot->end->vector.x, 0, 0.001) || !comparef(rot->end->vector.z, 0, 0.001))
			vectorbuilder(0, 1, 0, rot->axis);
	}
	else
		axisbuilder(ref, rot->end, rot->axis);
	rot->axis->ang = ang;
}

void	objrot(t_obj *obj, t_rot *rot, t_axis *end)
{
	int	i;

	vectorbuilder(0, 0, 1, rot->start);
	rot->end = end;
	flatanglehandler(rot, &obj->crdstm.oz);
	i = -1;
	while (++i < obj->dotsnum)
		quartrot(&obj->pos[i], rot->axis);
	i = -1;
	while (++i < obj->polynum)
		quartrot(&obj->poly[i].norm.vector, rot->axis);
	rot->start = &obj->crdstm.oz;
	rot->end = end;
	flatanglehandler(rot, NULL);
	quartrot(&obj->crdstm.oz.vector, rot->axis);
	quartrot(&obj->crdstm.oy.vector, rot->axis);
	quartrot(&obj->crdstm.ox.vector, rot->axis);
}

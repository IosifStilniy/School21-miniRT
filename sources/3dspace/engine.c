/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/06 21:59:27 by dcelsa           ###   ########.fr       */
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

void	flatanglehandler(t_axis *v1, t_axis *ref, t_axis *v2, t_axis *axis)
{
	float	ang;

	axisbuilder(v1, v2, axis);
	if (!comparef(axis->length, 0, 0.0001) && !comparef(axis->ang, M_PI, 0.001))
		return ;
	ang = axis->ang;
	axisbuilder(ref, v2, axis);
	axis->ang = ang;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_transformations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:45 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 22:33:54 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis)
{
	quartrot(&crdstm->oz.vector, zaxis);
	quartrot(&crdstm->oy.vector, zaxis);
	axisbuilder(&crdstm->oy.vector, &crdstm->oz.vector, &crdstm->ox);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
	if (!xyaxis)
		return ;
	quartrot(&crdstm->oz.vector, xyaxis);
	quartrot(&crdstm->oy.vector, xyaxis);
	axisbuilder(&crdstm->oy.vector, &crdstm->oz.vector, &crdstm->ox);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
}

void	objtoobjpos(t_cart *center, t_cart *dot)
{
	dot->x -= center->x;
	dot->y -= center->y;
	dot->z -= center->z;
}

static void exeptionhndlr(t_crdstm *crdstm)
{
	if (crdstm->oz.vector.x > 0)
	{
		vectorbuilder(0, 1, 0, &crdstm->ox);
		vectorbuilder(0, 0, 1, &crdstm->oy);
		return ;
	}
	vectorbuilder(0, 0, 1, &crdstm->ox);
	vectorbuilder(0, 1, 0, &crdstm->oy);
}

void	crdstmdefiner(t_crdstm *crdstm)
{
	t_axis	rot;

	vectorbuilder(1, 0, 0, &crdstm->ox);
	if (crdstm->ox.vector.x == fabsf(crdstm->oz.vector.x))
	{
		exeptionhndlr(crdstm);
		return ;
	}
	axisbuilder(&crdstm->ox.vector, &crdstm->oz.vector, &rot);
	if (rot.ang < M_PI_2)
	{
		negativevector(&rot.vector);
		rot.ang = M_PI_2 - rot.ang;
	}
	else if (!comparef(rot.ang, M_PI, M_PI / 180))
		rot.ang -= M_PI_2;
	else
		vectorbuilder(0, 1, 0, &rot);
	quartrot(&crdstm->ox.vector, &rot);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
}

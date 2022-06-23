/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/18 19:12:09 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	quartmult(t_axis *q, t_axis *p, t_axis *result)
{
	result->ang = q->ang * p->ang - q->vector.x * p->vector.x - q->vector.y * p->vector.y - q->vector.z * p->vector.z;
	result->vector.x = q->ang * p->vector.x + q->vector.x * p->ang + q->vector.y * p->vector.z - q->vector.z * p->vector.y;
	result->vector.y = q->ang * p->vector.y - q->vector.x * p->vector.z + q->vector.y * p->ang + q->vector.z * p->vector.x;
	result->vector.z = q->ang * p->vector.z + q->vector.x * p->vector.y - q->vector.y * p->vector.x + q->vector.z * p->ang;
}

void	quartrot(t_cart *pos, t_axis *axis)
{
	t_axis	axsr;
	t_axis	posq;
	t_axis	result;

	axsr.ang = cosf(axis->ang / 2);
	axsr.vector.x = sinf(axis->ang / 2) * axis->vector.x;
	axsr.vector.y = sinf(axis->ang / 2) * axis->vector.y;
	axsr.vector.z = sinf(axis->ang / 2) * axis->vector.z;
	posq.ang = 0;
	posq.vector.x = pos->x;
	posq.vector.y = pos->y;
	posq.vector.z = pos->z;
	quartmult(&axsr, &posq, &result);
	negativevector(&axsr.vector);
	quartmult(&result, &axsr, &posq);
	*pos = posq.vector;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:39:57 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 22:26:52 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	quartmult(t_axis *q, t_axis *p, t_axis *result)
{
	result->ang = q->ang * p->ang - q->vector.x * p->vector.x
		- q->vector.y * p->vector.y - q->vector.z * p->vector.z;
	result->vector.x = q->ang * p->vector.x + q->vector.x * p->ang
		+ q->vector.y * p->vector.z - q->vector.z * p->vector.y;
	result->vector.y = q->ang * p->vector.y - q->vector.x * p->vector.z
		+ q->vector.y * p->ang + q->vector.z * p->vector.x;
	result->vector.z = q->ang * p->vector.z + q->vector.x * p->vector.y
		- q->vector.y * p->vector.x + q->vector.z * p->ang;
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

void	dotcrdstmtrnsltn(t_cart *src, t_cart *dst, t_cart *scale,
	t_crdstm *crdstm)
{
	t_cart	defscale;

	cartbuilder(1, 1, 1, &defscale);
	if (!scale)
		scale = &defscale;
	dst->x = src->x * crdstm->ox.vector.x * scale->x;
	dst->y = src->x * crdstm->ox.vector.y * scale->x;
	dst->z = src->x * crdstm->ox.vector.z * scale->x;
	dst->x += src->y * crdstm->oy.vector.x * scale->y;
	dst->y += src->y * crdstm->oy.vector.y * scale->y;
	dst->z += src->y * crdstm->oy.vector.z * scale->y;
	dst->x += src->z * crdstm->oz.vector.x * scale->z;
	dst->y += src->z * crdstm->oz.vector.y * scale->z;
	dst->z += src->z * crdstm->oz.vector.z * scale->z;
}

t_bool	resizeobj(int arrow, t_intrfc *intrfc)
{
	t_vrtx		*vrtxs;
	t_dots		*d;
	t_cart		src[3];
	t_crdstm	world;
	int			i;

	if (!changecart(arrow, intrfc->selected->dots.scale, NULL,
			&intrfc->settings.size))
		return (FALSE);
	cartbuilder(0, 0, 1, &world.oz.vector);
	crdstmdefiner(&world);
	d = &intrfc->selected->dots;
	i = -1;
	while (++i < intrfc->selected->polys.polynum)
	{
		vrtxs = intrfc->selected->polys.poly[i].vrtxs;
		dotcrdstmtrnsltn(&d->dots[vrtxs[0].dot], &src[0], d->scale, &world);
		dotcrdstmtrnsltn(&d->dots[vrtxs[1].dot], &src[1], d->scale, &world);
		dotcrdstmtrnsltn(&d->dots[vrtxs[2].dot], &src[2], d->scale, &world);
		normbuilder(&src[0], &src[1], &src[2],
			&intrfc->selected->polys.poly[i].srcnorm);
	}
	return (TRUE);
}

void	engine(t_dots *dots, t_polys *polys, t_crdstm *crdstm, float *outframe)
{
	int		i;
	float	lng;

	*outframe = 0;
	i = -1;
	while (++i < dots->dotsnum)
	{
		dotcrdstmtrnsltn(&dots->dots[i], &dots->pos[i], dots->scale, crdstm);
		lng = vectorlength(&dots->pos[i]);
		if (*outframe < lng)
			*outframe = lng;
		dots->pos[i].x += crdstm->pos.x;
		dots->pos[i].y += crdstm->pos.y;
		dots->pos[i].z += crdstm->pos.z;
	}
	i = -1;
	while (++i < polys->polynum)
	{
		dotcrdstmtrnsltn(&polys->poly[i].srcnorm, &polys->poly[i].norm, NULL, crdstm);
		dotcrdstmtrnsltn(&polys->poly[i].vrtxs[0].srcnorm, &polys->poly[i].vrtxs[0].norm, NULL, crdstm);
		dotcrdstmtrnsltn(&polys->poly[i].vrtxs[1].srcnorm, &polys->poly[i].vrtxs[1].norm, NULL, crdstm);
		dotcrdstmtrnsltn(&polys->poly[i].vrtxs[2].srcnorm, &polys->poly[i].vrtxs[2].norm, NULL, crdstm);
	}
}

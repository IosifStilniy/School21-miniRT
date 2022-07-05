/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:41 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/05 15:07:22 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	vectorbuilder(float x, float y, float z, t_axis *vector)
{
	vector->length = sqrtf(x * x + y * y + z * z);
	if (vector->length)
		cartbuilder(x / vector->length, y / vector->length, z / vector->length,
			&vector->vector);
	else
		cartbuilder(0, 0, 0, &vector->vector);
	if (!vector->ang)
		vector->ang = DEFANG * M_PI / 180;
}

float	vectorlength(t_cart *dot)
{
	return (sqrtf(powf(dot->x, 2) + powf(dot->y, 2) + powf(dot->z, 2)));
}

void	normbuilder(t_cart *centraldot, t_cart *dot1, t_cart *dot2,
	t_cart *norm)
{
	t_cart	d1;
	t_cart	d2;
	float	length;

	if (centraldot)
	{
		d1 = *dot1;
		objtoobjpos(centraldot, &d1);
		dot1 = &d1;
		d2 = *dot2;
		objtoobjpos(centraldot, &d2);
		dot2 = &d2;
	}
	norm->x = dot1->y * dot2->z - dot1->z * dot2->y;
	norm->y = dot1->z * dot2->x - dot1->x * dot2->z;
	norm->z = dot1->x * dot2->y - dot1->y * dot2->x;
	length = sqrtf(powf(norm->x, 2) + powf(norm->y, 2) + powf(norm->z, 2));
	if (!length)
		return ;
	norm->x /= length;
	norm->y /= length;
	norm->z /= length;
}

void	axisbuilder(t_cart *v1, t_cart *v2, t_axis *axis)
{
	float	scalar;

	axis->vector.x = v1->y * v2->z - v1->z * v2->y;
	axis->vector.y = v1->z * v2->x - v1->x * v2->z;
	axis->vector.z = v1->x * v2->y - v1->y * v2->x;
	axis->length = sqrtf(powf(axis->vector.x, 2) + powf(axis->vector.y, 2)
			+ powf(axis->vector.z, 2));
	scalar = axis->length;
	scalar /= scalar * (scalar >= 1) + (scalar < 1);
	axis->ang = asinf(scalar);
	scalar = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
	if (scalar < 0)
		axis->ang = M_PI - axis->ang;
	if (!axis->length)
		return ;
	axis->vector.x /= axis->length;
	axis->vector.y /= axis->length;
	axis->vector.z /= axis->length;
}

void	vectorsizing(float newlength, t_cart *src, t_cart *vecres,
	float *lngthres)
{
	float	realsize;

	realsize = sqrtf(powf(src->x, 2) + powf(src->y, 2) + powf(src->z, 2));
	vecres->x = newlength * src->x / realsize;
	vecres->y = newlength * src->y / realsize;
	vecres->z = newlength * src->z / realsize;
	if (lngthres)
		*lngthres = newlength;
}

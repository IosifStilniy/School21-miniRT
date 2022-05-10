#include "minirt.h"

void	vectorbuilder(float x, float y, float z, t_axis *vector)
{
	vector->length = sqrtf(x * x + y * y + z * z);
	cartbuilder(x / vector->length, y / vector->length, z / vector->length,
		&vector->vector);
	if (!vector->ang)
		vector->ang = DEFANG * M_PI / 180;
}

void	normbuilder(t_cart *centraldot, t_cart *dot1, t_cart *dot2, t_axis *norm)
{
	t_axis	v1;
	t_axis	v2;

	vectorbuilder(dot1->x - centraldot->x, dot1->y - centraldot->y, dot1->z - centraldot->z, &v1);
	vectorbuilder(dot2->x - centraldot->x, dot2->y - centraldot->y, dot2->z - centraldot->z, &v2);
	axisbuilder(&v2, &v1, norm);
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

	axisbuilder(rot->start, rot->end, &rot->axis);
	ang = rot->axis.ang;
	if (!comparef(rot->axis.length, 0, 0.0001) && !comparef(rot->axis.ang, M_PI, 0.001))
		;
	else if (!ref)
	{
		vectorbuilder(1, 0, 0, &rot->axis);
		if (!comparef(rot->end->vector.x, 0, 0.001) || !comparef(rot->end->vector.z, 0, 0.001))
			vectorbuilder(0, 1, 0, &rot->axis);
	}
	else if (ref != rot->start)
		axisbuilder(ref, rot->end, &rot->axis);
	rot->axis.ang = ang;
}

void	vectortoobj(t_cart *from, t_cart *to, t_axis *vector)
{
	vector->vector.x = to->x - from->x;
	vector->vector.y = to->y - from->y;
	vector->vector.z = to->z - from->z;
	vector->length = sqrtf(powf(vector->vector.x, 2) + powf(vector->vector.y, 2) + powf(vector->vector.z, 2));
}

void	vectorsizing(float newlength, t_cart *src, t_axis *res)
{
	float	realsize;

	realsize = sqrtf(powf(src->x, 2) + powf(src->y, 2) + powf(src->z, 2));
	res->vector.x = newlength * src->x / realsize;
	res->vector.y = newlength * src->y / realsize;
	res->vector.z = newlength * src->z / realsize;
	res->length = newlength;
}

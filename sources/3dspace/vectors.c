#include "minirt.h"

void	rotfiller(t_rot *rot, t_axis *start, t_axis *end, t_axis *axis)
{
	rot->start = start;
	rot->end = end;
	rot->axis = axis;
}

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

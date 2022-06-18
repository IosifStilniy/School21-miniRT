#include "minirt.h"

void	cartbuilder(float x, float y, float z, t_cart *dot)
{
	dot->x = x;
	dot->y = y;
	dot->z = z;
}

void	cartcopy(t_cart *src, t_cart *dst, int count)
{
	while (count--)
	{
		cartbuilder(src->x, src->y, src->z, dst);
		src++;
		dst++;
	}
}

void	negativevector(t_cart *dot)
{
	dot->x *= -1;
	dot->y *= -1;
	dot->z *= -1;
}

void	vectodot(t_cart *vector, t_cart *start, t_bool normilize)
{
	float	length;

	vector->x += start->x;
	vector->y += start->y;
	vector->z += start->z;
	if (!normilize)
		return ;
	length = sqrtf(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2));
	if (comparef(length, 0, 0.001))
		return ;
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

void	dottranslation(t_cart *dot, t_cart *direction, float step)
{
	t_cart	res;

	vectorsizing(step, direction, &res, NULL);
	dot->x += res.x;
	dot->y += res.y;
	dot->z += res.z;
}

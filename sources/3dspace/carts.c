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

void	vectodot(t_cart *vector, t_cart *start)
{
	vector->x += start->x;
	vector->y += start->y;
	vector->z += start->z;
}

void	crdstmcopy(t_crdstm *src, t_crdstm *dst)
{
	cartcopy(&src->pos, &dst->pos, 1);
	cartcopy(&src->ox.vector, &dst->ox.vector, 1);
	cartcopy(&src->oy.vector, &dst->oy.vector, 1);
	cartcopy(&src->oz.vector, &dst->oz.vector, 1);
}

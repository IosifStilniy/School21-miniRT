#include "minirt.h"

static void	definelnguv(t_cart *ref, t_vrtx *dot, t_axis *axis)
{
	t_cart	xoz;

	cartbuilder(dot->dot.x, 0, dot->dot.z, &xoz);
	vectorsizing(1, &xoz, &xoz, NULL);
	axisbuilder(ref, &xoz, axis);
	dot->uv.x = 0.5f - 0.25f * axis->ang / M_PI_2;
	if (comparef(ref->y, 0, 0.001))
		dot->uv.x = 0.5f + 0.5f * (ref->x < 0);
	else if (ref->x < 0 && ref->y > 0)
		dot->uv.x = 0.25f * axis->ang / M_PI_2;
	else if (ref->x < 0 && ref->y < 0)
		dot->uv.x = 1.f - 0.25f * axis->ang / M_PI_2;
	else if (ref->x > 0 && ref->y < 0)
		dot->uv.x = 0.75f - 0.25f * axis->ang / M_PI_2;
}

void	spheremapping(t_vrtx *dots, int dotsnum)
{
	t_cart	xoy;
	t_cart	ref;
	t_axis	axis;
	int		i;

	dots[0].uv.x = 0.5f;
	dots[0].uv.y = 0.5f;
	dots[1].uv.x = 1.f;
	dots[1].uv.y = 0.5f;
	i = 1;
	while (++i < dotsnum)
	{
		vectorsizing(1, &dots[i].dot, &ref, NULL);
		cartbuilder(dots[i].dot.x, dots[i].dot.y, 0, &xoy);
		vectorsizing(1, &xoy, &xoy, NULL);
		axisbuilder(&ref, &xoy, &axis);
		dots[i].uv.y = (1.f + axis.ang / M_PI_2) * 0.5f;
		if (dots[i].dot.z > 0)
			dots[i].uv.y = (1.f - axis.ang / M_PI_2) * 0.5f;
		definelnguv(&ref, &dots[i], &axis);
	}
}

void	cylindermapping(t_vrtx *dots)
{
	int		i;

	i = -1;
	while (++i < RNDSGMNTS && ++dots)
	{
		dots->uv.y = 1.f;
		dots->uv.x = (float)i / (float)(RNDSGMNTS - 1);
	}
	i = -1;
	while (++i < RNDSGMNTS && ++dots)
	{
		dots->uv.y = 0.f;
		dots->uv.x = (float)i / (float)(RNDSGMNTS - 1);
	}
}

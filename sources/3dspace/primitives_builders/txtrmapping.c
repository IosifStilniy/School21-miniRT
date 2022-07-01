#include "minirt.h"

static void	definelnguv(t_cart *ref, t_vrtx *vrtx, t_axis *axis)
{
	t_cart	xoz;

	cartbuilder(ref->x, 0, ref->z, &xoz);
	vectorsizing(1, &xoz, &xoz, NULL);
	axisbuilder(ref, &xoz, axis);
	vrtx->uv.x = 0.5f - 0.25f * axis->ang / M_PI_2;
	if (comparef(ref->y, 0, 0.001))
		vrtx->uv.x = 0.5f + 0.5f * (ref->x < 0);
	else if (ref->x < 0 && ref->y > 0)
		vrtx->uv.x = 0.25f * axis->ang / M_PI_2;
	else if (ref->x < 0 && ref->y < 0)
		vrtx->uv.x = 1.f - 0.25f * axis->ang / M_PI_2;
	else if (ref->x > 0 && ref->y < 0)
		vrtx->uv.x = 0.75f - 0.25f * axis->ang / M_PI_2;
}

static void	spherepolymapping(t_vrtx *vrtx)
{
	t_cart	xoy;
	t_axis	axis;

	cartbuilder(vrtx->srcnorm.x, vrtx->srcnorm.y, 0, &xoy);
	vectorsizing(1, &xoy, &xoy, NULL);
	axisbuilder(&vrtx->srcnorm, &xoy, &axis);
	vrtx->uv.y = (1.f + axis.ang / M_PI_2) * 0.5f;
	if (vrtx->srcnorm.z > 0)
		vrtx->uv.y = (1.f - axis.ang / M_PI_2) * 0.5f;
	definelnguv(&vrtx->srcnorm, vrtx, &axis);
}

void	spheremapping(t_poly *polys, int polynum)
{
	int		i;

	i = -1;
	while (++i < polynum)
	{
		spherepolymapping(&polys[i].vrtxs[0]);
		spherepolymapping(&polys[i].vrtxs[1]);
		spherepolymapping(&polys[i].vrtxs[2]);
	}
}

static void	cylinderpolymapping(t_vrtx *vrtx, t_cart *dots)
{
	vrtx->uv.y = (dots[vrtx->dot].z < 0);
	if (!vrtx->dot || vrtx->dot == 1)
	{
		vrtx->uv.x = (dots[vrtx->dot].z < 0);
		return ;
	}
	vrtx->uv.x = (float)((vrtx->dot - 2) % RNDSGMNTS) / (RNDSGMNTS - 1);
}

void	cylindermapping(t_cart *dots, t_poly *polys, int polynum)
{
	int		i;

	i = -1;
	while (++i < polynum)
	{
		cylinderpolymapping(&polys[i].vrtxs[0], dots);
		cylinderpolymapping(&polys[i].vrtxs[1], dots);
		cylinderpolymapping(&polys[i].vrtxs[2], dots);
	}
}

#include "minirt.h"

static void	spherepolymapping(t_vrtx *vrtx)
{
	t_cart	ox;
	t_cart	oy;
	t_cart	oz;
	float	scalar;

	cartbuilder(1, 0, 0, &ox);
	cartbuilder(0, 1, 0, &oy);
	cartbuilder(0, 0, 1, &oz);
	ft_dotprod(&vrtx->srcnorm, &oz, &scalar);
	vrtx->uv.y = 0.5f - 0.5f * scalar;
	ft_dotprod(&vrtx->srcnorm, &oy, &scalar);
	vrtx->uv.x = 0.5f - 0.25f * scalar;
	ft_dotprod(&vrtx->srcnorm, &ox, &scalar);
	vrtx->uv.x -= 0.25f * scalar;
	// vrtx->uv.x = 0.5f + atan2f(vrtx->srcnorm.z, vrtx->srcnorm.x) / (2 * M_PI);
	// vrtx->uv.y = 0.5f + asinf(vrtx->srcnorm.y) / M_PI;
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

#include "minirt.h"

void	repairbackpatch(t_vrtx vrtxs[3])
{
	int		i;
	int		j;

	i = -1;
	while (++i < 3)
	{
		if (vrtxs[i].dot < 2 || !comparef(vrtxs[i].uv.x, 0, 0.001))
			continue ;
		j = -1;
		while (++j < 3)
			if (vrtxs[j].uv.x > 0.5f)
				vrtxs[i].uv.x = 1.f;
	}
}

static void	cylinderpolymapping(t_vrtx *vrtx, t_cart *dots)
{
	vrtx->uv.y = (dots[vrtx->dot].z > 0);
	if (!vrtx->dot || vrtx->dot == 1)
	{
		vrtx->uv.x = (dots[vrtx->dot].z > 0);
		return ;
	}
	vrtx->uv.x = (float)((vrtx->dot - 2) % RNDSGMNTS) / RNDSGMNTS;
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
		repairbackpatch(polys[i].vrtxs);
	}
}

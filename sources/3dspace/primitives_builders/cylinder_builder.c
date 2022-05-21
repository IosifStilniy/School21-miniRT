#include "minirt.h"

static void	surfdefiner(int strtnm, int shift, t_poly *polys, t_vrtx *dots)
{
	int		i;
	int		dotindxs[4];

	i = 0;
	while (++i < RNDSGMNTS)
	{
		dotindxs[0] = i;
		dotindxs[1] = shift + i;
		dotindxs[2] = shift + i - 1;
		dotindxs[3] = i - 1;
		surfing(polys + i - 1, dotindxs, 4, dots);
		polys[i - 1].txtr = NULL;
	}
	dotindxs[0] = strtnm;
	dotindxs[1] = shift;
	dotindxs[2] = shift + RNDSGMNTS - 1;
	dotindxs[3] = strtnm + RNDSGMNTS - 1 - 1;
	surfing(polys + --i, dotindxs, 4, dots);
	polys[i].txtr = NULL;
}

static void	buttsurf(int central, int strnum, t_poly *polys, t_vrtx *dots)
{
	int		i;
	int		dotindxs[3];

	i = -1;
	while (++i < RNDSGMNTS - 1)
	{
		dotindxs[0] = central;
		dotindxs[1] = strnum + i;
		dotindxs[2] = strnum + i + 1;
		surfing(polys + i, dotindxs, 3, dots);
		polys[i].txtr = NULL;
	}
	dotindxs[0] = central;
	dotindxs[1] = strnum + i;
	dotindxs[2] = strnum;
	surfing(polys + i, dotindxs, 3, dots);
	polys[i].txtr = NULL;
}

float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height)
{
	int	i;

	dots->dotsnum = RNDSGMNTS * 2 + 2;
	polys->polynum = RNDSGMNTS * 3;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	cartbuilder(0, 0, -height / 2, dots);
	cartbuilder(0, 0, height / 2, dots + 1);
	circledotsfiller(&dots->dots[2], radius, NULL, FALSE);
	circledotsfiller(&dots->dots[RNDSGMNTS + 2], radius, NULL, FALSE);
	i = -1;
	while (++i < RNDSGMNTS)
	{
		dots->dots[2 + i].dot.z = -height / 2;
		dots->dots[2 + RNDSGMNTS + i].dot.z = height / 2;
	}
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	surfdefiner(2, RNDSGMNTS + 2, polys->poly, dots->dots);
	buttsurf(0, 2, &polys->poly[RNDSGMNTS], dots->dots);
	buttsurf(1, RNDSGMNTS + 2, &polys->poly[2 * RNDSGMNTS], dots->dots);
	return (sqrtf(powf(height / 2, 2) + powf(radius, 2)));
}

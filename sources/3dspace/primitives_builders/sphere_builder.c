#include "minirt.h"

static void	surfdefiner(t_vrtx *dots, t_poly **poly, int dotnum, void *txtr)
{
	int	dotindxs[3];

	dotindxs[0] = dotnum;
	dotindxs[1] = dotnum - 1 - (RNDSGMNTS - 2);
	dotindxs[2] = dotnum - 1;
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
	dotindxs[1] = dotnum - (RNDSGMNTS - 2);
	dotindxs[2] = dotnum - 1 - (RNDSGMNTS - 2);
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
}

static int	beltsurfing(t_vrtx *dots, t_poly **polys, int dotnum, void *txtr)
{
	int	dotsinround;

	dotsinround = 0;
	while (++dotsinround < (RNDSGMNTS - 2) / 2)
		surfdefiner(dots, polys, ++dotnum, txtr);
	return (++dotnum);
}

static void	dotfiller(t_vrtx *dots, t_poly *polys, float radius, void *txtr)
{
	int		lttd;
	int		dotnum;
	float	step;
	t_axis	rotltd;

	definepols(dots, radius, NULL);
	dotnum = 2 + circledotsfiller(&dots[2], radius, NULL, TRUE);
	vectorbuilder(1, 0, 0, &rotltd);
	step = 2 * M_PI / RNDSGMNTS;
	lttd = 0;
	rotltd.ang = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		rotltd.ang += step;
		circledotsfiller(&dots[dotnum], radius, &rotltd, TRUE);
		dotnum = beltsurfing(dots, &polys, dotnum, txtr);
		dotnum = beltsurfing(dots, &polys, dotnum, txtr);
	}
}

static void	jointing(t_vrtx *dots, t_poly *polys, int dotsnum, void *txtr)
{
	int		i;
	int		dotindxs[4];

	i = 0;
	while (++i < RNDSGMNTS - 2)
	{
		if (i == (RNDSGMNTS - 2) / 2)
			continue ;
		dotindxs[0] = i + 2;
		dotindxs[1] = i - 1 + 2;
		dotindxs[2] = dotsnum - 1 - (i - 1);
		repairspherenormal(++polys, dotindxs, dots, txtr);
		dotindxs[1] = dotsnum - 1 - (i - 1);
		dotindxs[2] = dotsnum - 1 - i;
		repairspherenormal(++polys, dotindxs, dots, txtr);
	}
}

float	spherebuilder(t_dots *dots, t_polys *polys, float radius)
{
	int		lttd;
	int		polyshift;
	t_poly	*buf;

	dots->dotsnum = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2;
	polys->polynum = RNDSGMNTS * (RNDSGMNTS - 4) + 2 * RNDSGMNTS;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	dotfiller(dots->dots, polys->poly - 1, radius, polys->txtr);
	polyshift = ((RNDSGMNTS - 2) * (RNDSGMNTS / 2 - 2)) * 2 - 1;
	if (polyshift > 0)
		jointing(dots->dots, polys->poly + polyshift, dots->dotsnum, polys->txtr);
	polyshift = polys->polynum - RNDSGMNTS * 2 - 1;
	buf = polys->poly + polyshift;
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
		polarsurfing(dots->dots, &buf, lttd, polys->txtr);
	polarjointing(dots->dots, polys->poly + polys->polynum - 5, polys->txtr, dots->dotsnum);
	return (radius);
}

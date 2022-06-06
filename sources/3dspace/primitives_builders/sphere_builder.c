#include "minirt.h"

static void	surfdefiner(t_vrtx *dots, t_poly **poly, int dotnum, void *txtr)
{
	int	dotindxs[3];

	dotindxs[0] = dotnum;
	dotindxs[1] = dotnum - 1;
	dotindxs[2] = dotnum - 1 - (RNDSGMNTS - 2);
	surfing(*poly, dotindxs, dots, txtr);
	dotindxs[1] = dotnum - 1 - (RNDSGMNTS - 2);
	dotindxs[2] = dotnum - (RNDSGMNTS - 2);
	surfing(++(*poly), dotindxs, dots, txtr);
	(*poly)++;
}

static int	beltsurfing(t_vrtx *dots, t_poly **polys, int dotnum, void *txtr)
{
	int	dotsinround;

	dotsinround = (RNDSGMNTS - 2) / 2 - 1;
	while (dotsinround--)
		surfdefiner(dots, polys, dotnum++, txtr);
	return (dotnum);
}

static void	dotfiller(t_vrtx *dots, t_poly *polys, float radius, void *txtr)
{
	int		lttd;
	int		dotnum;
	float	step;
	t_axis	rotltd;

	definepols(dots, radius, NULL);
	circledotsfiller(&dots[2], radius, NULL, TRUE);
	dotnum = RNDSGMNTS;
	vectorbuilder(0, 1, 0, &rotltd);
	step = 2 * M_PI / RNDSGMNTS;
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		rotltd.ang = step * lttd;
		circledotsfiller(&dots[dotnum], radius, &rotltd, TRUE);
		dotnum = beltsurfing(dots, &polys, ++dotnum, txtr);
		dotnum = beltsurfing(dots, &polys, ++dotnum, txtr) + 1;
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
		surfing(++polys, dotindxs, dots, txtr);
		dotindxs[1] = dotsnum - 1 - (i - 1);
		dotindxs[2] = dotsnum - 1 - i;
		surfing(++polys, dotindxs, dots, txtr);
	}
}

float	spherebuilder(t_dots *dots, t_polys *polys, float radius)
{
	int		lttd;
	int		polyshift;
	void	*buf;

	dots->dotsnum = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2;
	polys->polynum = RNDSGMNTS * (RNDSGMNTS - 4) + 2 * RNDSGMNTS;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	dotfiller(dots->dots, polys->poly, radius, polys->txtr);
	polyshift = RNDSGMNTS * (RNDSGMNTS - 5) - 2;
	if (polyshift > 0)
		jointing(dots->pos, polys->poly + polyshift, dots->dotsnum, polys->txtr);
	polyshift = RNDSGMNTS * (RNDSGMNTS - 4) - 2;
	buf = polys->poly + polyshift;
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
		polarsurfing(dots, buf, lttd, polys->txtr);
	polarjointing(dots, polys->poly + polys->polynum - 5, polys->txtr, dots->dotsnum);
	return (radius);
}

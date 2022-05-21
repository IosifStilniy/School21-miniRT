#include "minirt.h"

static void	surfdefiner(t_vrtx *dots, t_poly *poly, int dotnum, int lttd)
{
	int	i;
	int	dotindxs[4];

	dotindxs[0] = dotnum;
	dotindxs[1] = dotnum - 1;
	dotindxs[2] = dotnum - 1 - (RNDSGMNTS - 2) * lttd;
	dotindxs[3] = dotnum - (RNDSGMNTS - 2) * lttd;
	surfing(poly, dotindxs, 4, dots);
	poly->txtr = NULL;
}

static int	dotfiller(t_vrtx *dots, t_cart *pos, t_poly *polys, float radius)
{
	int		lttd;
	int		dotnum;
	int		dotsinround;
	int		polyindx;
	t_axis	rotltd;

	definepols(dots, radius, NULL);
	dotnum = 2 + circledotsfiller(&dots[2], radius, NULL, TRUE);
	vectorbuilder(0, 1, 0, &rotltd);
	polyindx = -1;
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		rotltd.ang = 2 * M_PI / RNDSGMNTS * lttd;
		dotsinround = circledotsfiller(&dots[dotnum], radius, &rotltd, TRUE);
		while (--dotsinround)
			if ((dotsinround == (RNDSGMNTS - 2) / 2 + 1 && ++dotnum) || TRUE)
				surfdefiner(dots, &polys[++polyindx], dotnum++, lttd);
		dotnum++;
	}
	return (polyindx);
}

static int	jointing(t_vrtx *dots, t_poly *polys, int dotsnum, int indx)
{
	int		i;
	int		j;
	int		dotindxs[4];

	i = 0;
	while (++i < RNDSGMNTS - 2)
	{
		if (i == (RNDSGMNTS - 2) / 2 - 1)
			continue ;
		dotindxs[0] = i + 2;
		dotindxs[1] = i - 1 + 2;
		dotindxs[2] = dotsnum - 2 - 1 - (i - 1) + 2;
		dotindxs[3] = dotsnum - 2 - 1 - i + 2;
		surfing(polys + ++indx, dotindxs, 4, dots);
		polys[indx].txtr = NULL;
	}
	frontpsurfpatch(dots, &polys[++indx], FALSE, 0);
	frontpsurfpatch(dots, &polys[++indx], TRUE, 0);
	backpsurfpatch(dots, &polys[++indx], FALSE, 0);
	backpsurfpatch(dots, &polys[++indx], TRUE, 0);
	return (indx);
}

float	spherebuilder(t_dots *dots, t_polys *polys, float radius)
{
	int	lttd;
	int	polyindx;

	dots->dotsnum = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2;
	polys->polynum = RNDSGMNTS / 2 * (RNDSGMNTS - 3) + 2 * RNDSGMNTS;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	polyindx = dotfiller(dots->dots, dots->pos, polys->poly, radius);
	polyindx = jointing(dots->pos, &polys->poly, dots->dotsnum, polyindx);
	lttd = 0;
	while (++lttd < RNDSGMNTS / 2)
	{
		frontpsurfpatch(dots, &polys[++polyindx], FALSE, lttd);
		frontpsurfpatch(dots, &polys[++polyindx], TRUE, lttd);
		backpsurfpatch(dots, &polys[++polyindx], FALSE, lttd);
		backpsurfpatch(dots, &polys[++polyindx], TRUE, lttd);
	}
	return (radius);
}

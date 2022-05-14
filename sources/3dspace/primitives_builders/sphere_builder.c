#include "minirt.h"

static void	surfdefiner(t_vrtx *dots, t_poly *poly, int dotnum, int lttd)
{
	poly->dotcount = 4;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = dotnum;
	poly->dots[1] = dotnum - 1;
	poly->dots[2] = dotnum - 1 - (RNDSGMNTS - 2) * lttd;
	poly->dots[3] = dotnum - (RNDSGMNTS - 2) * lttd;
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[3]].dot, &poly->srcnorm);
	setvrtxpolynorms(&poly->srcnorm, dots, poly->dots, poly->dotcount);
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

	i = 0;
	while (++i < RNDSGMNTS - 2)
	{
		if (i == (RNDSGMNTS - 2) / 2 - 1)
			continue ;
		polys[++indx].dotcount = 4;
		polys[indx].dots = malloc(sizeof(*polys->dots) * polys->dotcount);
		polys[indx].dots[0] = i + 2;
		polys[indx].dots[1] = i - 1 + 2;
		polys[indx].dots[2] = dotsnum - 2 - 1 - (i - 1) + 2;
		polys[indx].dots[3] = dotsnum - 2 - 1 - i + 2;
		normbuilder(&dots[i + 2].dot, &dots[i - 1 + 2].dot, &dots[dotsnum - 3 - i + 2].dot, &polys[indx].srcnorm);
		setvrtxpolynorms(&polys[indx].srcnorm, dots, polys[indx].dots, polys[indx].dotcount);
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
	lttd = -1;
	while (++lttd < dots->dotsnum)
		vrtxnormdefiner(dots->dots + lttd);
	return (radius);
}

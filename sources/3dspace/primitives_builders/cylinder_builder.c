#include "minirt.h"

static void	surfdefiner(int strtnm, int shift, t_poly *polys, t_vrtx *dots)
{
	int		i;

	i = 0;
	while (++i < RNDSGMNTS)
	{
		polys[i - 1].dotcount = 4;
		polys[i - 1].dots = malloc(sizeof(*polys->dots) * polys[i - 1].dotcount);
		polys[i - 1].dots[0] = i;
		polys[i - 1].dots[1] = shift + i;
		polys[i - 1].dots[2] = shift + i - 1;
		polys[i - 1].dots[3] = i - 1;
		normbuilder(&dots[shift + i].dot, &dots[shift + i - 1].dot, &dots[i].dot, &polys[i - 1].srcnorm);
		setvrtxpolynorms(&polys[i - 1].srcnorm, dots, polys[i - 1].dots, polys[i - 1].dotcount);
		polys[i - 1].txtr = NULL;
	}
	polys[--i].dotcount = 4;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = strtnm;
	polys[i].dots[1] = shift;
	polys[i].dots[2] = shift + RNDSGMNTS - 1;
	polys[i].dots[3] = strtnm + RNDSGMNTS - 1 - 1;
	normbuilder(&dots[strtnm].dot, &dots[shift].dot, &dots[polys[i].dots[3]].dot, &polys[i].srcnorm);
	setvrtxpolynorms(&polys[i].srcnorm, dots, polys[i].dots, polys[i].dotcount);
	polys[i].txtr = NULL;
}

static void	buttsurf(int central, int strnum, t_poly *polys, t_vrtx *dots)
{
	int	i;

	i = -1;
	while (++i < RNDSGMNTS - 1)
	{
		polys[i].dotcount = 3;
		polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
		polys[i].dots[0] = central;
		polys[i].dots[1] = strnum + i;
		polys[i].dots[2] = strnum + i + 1;
		normbuilder(&dots[central].dot, &dots[strnum + i].dot, &dots[strnum + i + 1].dot, &polys[i].srcnorm);
		setvrtxpolynorms(&polys[i].srcnorm, dots, polys[i].dots, polys[i].dotcount);
		polys[i].txtr = NULL;
	}
	polys[i].dotcount = 3;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = central;
	polys[i].dots[1] = strnum + i;
	polys[i].dots[2] = strnum;
	normbuilder(&dots[central].dot, &dots[strnum + i].dot, &dots[strnum].dot, &polys[i].srcnorm);
	setvrtxpolynorms(&polys[i].srcnorm, dots, polys[i].dots, polys[i].dotcount);
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
	i = -1;
	while (++i < dots->dotsnum)
		vrtxnormdefiner(dots->dots + i);
	return (sqrtf(powf(height / 2, 2) + powf(radius, 2)));
}

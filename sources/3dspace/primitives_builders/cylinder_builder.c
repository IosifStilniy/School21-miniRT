#include "minirt.h"

static void	surfdefiner(t_cart *dots, t_cart *shift, t_poly *polys)
{
	int		i;

	i = 0;
	while (++i < RNDSGMNTS)
	{
		polys[i - 1].dotcount = 4;
		polys[i - 1].dots = malloc(sizeof(*polys->dots) * polys[i - 1].dotcount);
		polys[i - 1].dots[0] = &dots[i];
		polys[i - 1].dots[1] = &shift[i];
		polys[i - 1].dots[2] = &shift[i - 1];
		polys[i - 1].dots[3] = &dots[i - 1];
		normbuilder(&shift[i], &shift[i - 1], &dots[i], &polys[i - 1].norm);
		polys[i - 1].txtr = NULL;
	}
	polys[--i].dotcount = 4;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = dots;
	polys[i].dots[1] = shift;
	polys[i].dots[2] = &shift[RNDSGMNTS - 1];
	polys[i].dots[3] = &dots[RNDSGMNTS - 1 - 1];
	normbuilder(polys[i].dots, &polys[i].dots[1], &polys[i].dots[3], &polys[i].norm);
	polys[i].txtr = NULL;
}

static void	buttsurf(t_cart *central, t_cart *dots, t_poly *polys)
{
	int	i;

	i = -1;
	while (++i < RNDSGMNTS - 1)
	{
		polys[i].dotcount = 3;
		polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
		polys[i].dots[0] = central;
		polys[i].dots[1] = &dots[i];
		polys[i].dots[2] = &dots[i + 1];
		normbuilder(polys[i].dots, &polys[i].dots[1], &polys[i].dots[2], &polys[i].norm);
		polys[i].txtr = NULL;
	}
	polys[i].dotcount = 3;
	polys[i].dots = malloc(sizeof(*polys->dots) * polys[i].dotcount);
	polys[i].dots[0] = central;
	polys[i].dots[1] = &dots[i];
	polys[i].dots[2] = dots;
	normbuilder(polys[i].dots, &polys[i].dots[1], &polys[i].dots[2], &polys[i].norm);
	polys[i].txtr = NULL;
}

float	cylinderbuilder(t_dots *dots, t_polys *polys, float radius, float height)
{
	int	i;

	dots->dotsnum = RNDSGMNTS * 2 + 2;
	polys->polynum = RNDSGMNTS * 3;
	dots->dots = malloc(sizeof(*dots->dots) * dots->dotsnum);
	dots->pos = malloc(sizeof(*dots->pos) * dots->dotsnum);
	cartbuilder(0, 0, -height / 2, dots);
	cartbuilder(0, 0, height / 2, dots + 1);
	circledotsfiller(&dots->dots[2], radius, NULL, FALSE);
	circledotsfiller(&dots->dots[RNDSGMNTS + 2], radius, NULL, FALSE);
	i = -1;
	while (++i < RNDSGMNTS)
	{
		dots->dots[2 + i].z = -height / 2;
		dots->dots[2 + RNDSGMNTS + i].z = height / 2;
	}
	polys->poly = malloc(sizeof(*polys->poly) * polys->polynum);
	surfdefiner(&dots->dots[2], &dots->dots[RNDSGMNTS + 2], polys->poly);
	buttsurf(&dots->dots[0], &dots->dots[2], &polys->poly[RNDSGMNTS]);
	buttsurf(&dots->dots[1], &dots->dots[RNDSGMNTS + 2], &polys->poly[2 * RNDSGMNTS]);
	return (sqrtf(powf(height / 2, 2) + powf(radius, 2)));
}

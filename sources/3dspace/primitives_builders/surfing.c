#include "minirt.h"

void	surfing(t_poly *poly, int *dotindxs, int dotcount, t_vrtx *dots)
{
	int	i;

	poly->dotcount = dotcount;
	poly->dots = malloc(sizeof(*poly->dots) * dotcount);
	i = -1;
	while (++i < poly->dotcount)
		poly->dots[i] = dotindxs[i];
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[poly->dotcount - 1]].dot, &poly->srcnorm);
	i = -1;
	while (++i < poly->dotcount)
		vectodot(&dots[poly->dots[i]].norm, &poly->norm, TRUE);
}

void	backpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd)
{
	int	i;

	poly->dotcount = 3;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = south;
	poly->dots[1] = lttd * (RNDSGMNTS - 2) + 2 + RNDSGMNTS - 3;
	if (south)
		poly->dots[1] -= (RNDSGMNTS - 2) / 2 - 1;
	poly->dots[2] = (lttd - 1) * (RNDSGMNTS - 2) + 2;
	if (!lttd)
		poly->dots[2] = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2 - (RNDSGMNTS - 2) - 1;
	if (south)
		poly->dots[2] += (RNDSGMNTS - 2) / 2 - 1;
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	i = -1;
	while (++i < poly->dotcount)
		vectodot(&dots[poly->dots[i]].norm, &poly->srcnorm, TRUE);
	poly->txtr = NULL;
}

void	frontpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd)
{
	int	i;

	poly->dotcount = 3;
	poly->dots = malloc(sizeof(*poly->dots) * poly->dotcount);
	poly->dots[0] = south;
	poly->dots[1] = lttd * (RNDSGMNTS - 2) + 2;
	if (south)
		poly->dots[1] += (RNDSGMNTS - 2) / 2 - 1;
	poly->dots[2] = (lttd - 1) * (RNDSGMNTS - 2) + 2;
	if (!lttd)
		poly->dots[2] = RNDSGMNTS / 2 * (RNDSGMNTS - 2) + 2 - 1;
	if (south)
		poly->dots[2] -= (RNDSGMNTS - 2) / 2 - 1;
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	i = -1;
	while (++i < poly->dotcount)
		vectodot(&dots[poly->dots[i]].norm, &poly->srcnorm, TRUE);
	poly->txtr = NULL;
}

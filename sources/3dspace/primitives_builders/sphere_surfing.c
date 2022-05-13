#include "minirt.h"

void	backpsurfpatch(t_cart *dots, t_poly *poly, t_bool south, int lttd)
{
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
	normbuilder(&dots[poly->dots[0]], &dots[poly->dots[1]], &dots[poly->dots[2]], &poly->srcnorm);
	poly->txtr = NULL;
}

void	frontpsurfpatch(t_cart *dots, t_poly *poly, t_bool south, int lttd)
{
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
	normbuilder(&dots[poly->dots[0]], &dots[poly->dots[1]], &dots[poly->dots[2]], &poly->srcnorm);
	poly->txtr = NULL;
}

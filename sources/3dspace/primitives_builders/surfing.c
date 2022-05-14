#include "minirt.h"

void	vrtxnormdefiner(t_vrtx *dot)
{
	t_list	*crsr;
	float	length;

	cartbuilder(0, 0, 0, &dot->norm);
	crsr = dot->polynorms;
	while (crsr)
	{
		vectodot(&dot->norm, cartcast(crsr));
		crsr = crsr->next;
	}
	length = sqrtf(powf(dot->norm.x, 2) + powf(dot->norm.y, 2) + powf(dot->norm.z, 2));
	if (!length)
		return ;
	dot->norm.x /= length;
	dot->norm.y /= length;
	dot->norm.z /= length;
}

void	setvrtxpolynorms(t_cart *norm, t_vrtx *dots, int *indxs, int count)
{
	while (count--)
		ft_lstadd_front(&dots[indxs[count]].polynorms, ft_lstnew(norm));
}

void	backpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd)
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
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	setvrtxpolynorms(&poly->srcnorm, dots, poly->dots, poly->dotcount);
	poly->txtr = NULL;
}

void	frontpsurfpatch(t_vrtx *dots, t_poly *poly, t_bool south, int lttd)
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
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	setvrtxpolynorms(&poly->srcnorm, dots, poly->dots, poly->dotcount);
	poly->txtr = NULL;
}

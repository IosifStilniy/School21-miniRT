#include "minirt.h"

void	definespherevrtxs(t_cart *dots, t_poly *polys, int polynum)
{
	int		i;

	i = -1;
	while (++i < polynum)
	{
		polys[i].interpolate = TRUE;
		vectorsizing(1, &dots[polys[i].vrtxs[0].dot], &polys[i].vrtxs[0].srcnorm, NULL);
		vectorsizing(1, &dots[polys[i].vrtxs[1].dot], &polys[i].vrtxs[1].srcnorm, NULL);
		vectorsizing(1, &dots[polys[i].vrtxs[2].dot], &polys[i].vrtxs[2].srcnorm, NULL);
	}	
}

void	buttvrtxing(t_poly *poly, t_data *txtr)
{
	poly->txtr = txtr;
	poly->interpolate = FALSE;
	poly->vrtxs[0].srcnorm = poly->srcnorm;
	poly->vrtxs[1].srcnorm = poly->srcnorm;
	poly->vrtxs[2].srcnorm = poly->srcnorm;
}

void	definecylindervrtxs(t_cart *dots, t_poly *polys, int polynum, t_data *txtr)
{
	t_cart	norm;
	t_cart	*dot;
	int		i;

	i = -1;
	while (++i < RNDSGMNTS * 2)
	{
		polys[i].txtr = txtr;
		polys[i].interpolate = TRUE;
		dot = &dots[polys[i].vrtxs[0].dot];
		cartbuilder(dot->x, dot->y, 0, &norm);
		vectorsizing(1, &norm, &polys[i].vrtxs[0].srcnorm, NULL);
		dot = &dots[polys[i].vrtxs[1].dot];
		cartbuilder(dot->x, dot->y, 0, &norm);
		vectorsizing(1, &norm, &polys[i].vrtxs[1].srcnorm, NULL);
		dot = &dots[polys[i].vrtxs[2].dot];
		cartbuilder(dot->x, dot->y, 0, &norm);
		vectorsizing(1, &norm, &polys[i].vrtxs[2].srcnorm, NULL);
	}
	while (i < polynum)
		buttvrtxing(&polys[i++], txtr);
}

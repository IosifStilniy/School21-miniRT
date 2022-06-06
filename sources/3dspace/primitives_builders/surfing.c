#include "minirt.h"

void	surfing(t_poly *poly, int *dotindxs, t_vrtx *dots, void *txtr)
{
	poly->dots[0] = dotindxs[0];
	poly->dots[1] = dotindxs[1];
	poly->dots[2] = dotindxs[2];
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	vectodot(&dots[poly->dots[0]].norm, &poly->srcnorm, TRUE);
	vectodot(&dots[poly->dots[1]].norm, &poly->srcnorm, TRUE);
	vectodot(&dots[poly->dots[2]].norm, &poly->srcnorm, TRUE);
	poly->txtr = txtr;
}

void	polarsurfing(t_vrtx *dots, t_poly **poly, int lttd, void *txtr)
{
	int	dotindxs[3];

	dotindxs[0] = 0;
	dotindxs[1] = (lttd - 1) * (RNDSGMNTS - 2) + 2;
	dotindxs[2] = lttd * (RNDSGMNTS - 2) + 2;
	surfing(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 0;
	dotindxs[1] += (RNDSGMNTS - 2) - 1;
	dotindxs[2] += (RNDSGMNTS - 2) - 1;
	surfing(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1] = lttd * (RNDSGMNTS - 2) + (RNDSGMNTS - 2) / 2 - 1 + 2;
	dotindxs[2] = (lttd - 1) * (RNDSGMNTS - 2) + (RNDSGMNTS - 2) / 2 - 1 + 2;
	surfing(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1]++;
	dotindxs[2]++;
	surfing(++(*poly), dotindxs, dots, txtr);
}

void	polarjointing(t_vrtx *dots, t_poly *poly, void *txtr, int dotnum)
{
	int	dotindxs[3];

	dotindxs[0] = 0;
	dotindxs[1] = dotnum - 1;
	dotindxs[2] = 2;
	surfing(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 0;
	dotindxs[1] -= (RNDSGMNTS - 2) / 2;
	dotindxs[2] += (RNDSGMNTS - 2) / 2;
	surfing(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1] = 2 + (RNDSGMNTS - 2) / 2 - 1;
	dotindxs[2] = dotnum - (RNDSGMNTS - 2) / 2;
	surfing(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1]++;
	dotindxs[2]--;
	surfing(++poly, dotindxs, dots, txtr);
}

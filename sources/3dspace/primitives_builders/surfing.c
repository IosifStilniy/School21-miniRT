#include "minirt.h"

void	surfing(t_poly *poly, int dotindxs[3], t_vrtx *dots, void *txtr)
{
	poly->dots[0] = dotindxs[0];
	poly->dots[1] = dotindxs[1];
	poly->dots[2] = dotindxs[2];
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	vectodot(&dots[poly->dots[0]].norm, &poly->srcnorm);
	vectodot(&dots[poly->dots[1]].norm, &poly->srcnorm);
	vectodot(&dots[poly->dots[2]].norm, &poly->srcnorm);
	poly->txtr = txtr;
}

void	repairspherenormal(t_poly *poly, int dotindxs[3], t_vrtx *dots, void *txtr)
{
	t_cart	ref;
	t_axis	res;
	int		buf;

	surfing(poly, dotindxs, dots, txtr);
	ref = dots[dotindxs[0]].dot;
	vectorsizing(1, &ref, &ref, NULL);
	axisbuilder(&ref, &poly->srcnorm, &res);
	if (res.ang < M_PI_2)
		return ;
	buf = poly->dots[2];
	poly->dots[2] = poly->dots[1];
	poly->dots[1] = buf;
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	vectorsizing(2, &poly->srcnorm, &ref, NULL);
	vectodot(&dots[poly->dots[0]].norm, &ref);
	vectodot(&dots[poly->dots[1]].norm, &ref);
	vectodot(&dots[poly->dots[2]].norm, &ref);
}

void	polarsurfing(t_vrtx *dots, t_poly **poly, int lttd, void *txtr)
{
	int	dotindxs[3];

	dotindxs[0] = 0;
	dotindxs[1] = (lttd - 1) * (RNDSGMNTS - 2) + 2;
	dotindxs[2] = lttd * (RNDSGMNTS - 2) + 2;
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 0;
	dotindxs[1] += (RNDSGMNTS - 2) - 1;
	dotindxs[2] += (RNDSGMNTS - 2) - 1;
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1] = lttd * (RNDSGMNTS - 2) + (RNDSGMNTS - 2) / 2 - 1 + 2;
	dotindxs[2] = (lttd - 1) * (RNDSGMNTS - 2) + (RNDSGMNTS - 2) / 2 - 1 + 2;
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1]++;
	dotindxs[2]++;
	repairspherenormal(++(*poly), dotindxs, dots, txtr);
}

void	polarjointing(t_vrtx *dots, t_poly *poly, void *txtr, int dotnum)
{
	int	dotindxs[3];

	dotindxs[0] = 0;
	dotindxs[1] = dotnum - 1;
	dotindxs[2] = 2;
	repairspherenormal(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 0;
	dotindxs[1] -= (RNDSGMNTS - 2) - 1;
	dotindxs[2] += (RNDSGMNTS - 2) - 1;
	repairspherenormal(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1] = 2 + (RNDSGMNTS - 2) / 2 - 1;
	dotindxs[2] = dotnum - (RNDSGMNTS - 2) / 2;
	repairspherenormal(++poly, dotindxs, dots, txtr);
	dotindxs[0] = 1;
	dotindxs[1]++;
	dotindxs[2]--;
	repairspherenormal(++poly, dotindxs, dots, txtr);
}

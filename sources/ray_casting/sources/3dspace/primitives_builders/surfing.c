#include "minirt.h"

void	surfing(t_poly *poly, int dotindxs[3], t_vrtx *dots, void *txtr)
{
	poly->dots[0] = dotindxs[0];
	poly->dots[1] = dotindxs[1];
	poly->dots[2] = dotindxs[2];
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
	poly->txtr = txtr;
}

void	repairspherenormal(t_poly *poly, int dotindxs[3], t_vrtx *dots, void *txtr)
{
	int		buf;

	surfing(poly, dotindxs, dots, txtr);
	if (ft_get_dot_product(&dots[dotindxs[0]].dot, &poly->srcnorm) > 0)
		return ;
	buf = poly->dots[2];
	poly->dots[2] = poly->dots[1];
	poly->dots[1] = buf;
	normbuilder(&dots[poly->dots[0]].dot, &dots[poly->dots[1]].dot, &dots[poly->dots[2]].dot, &poly->srcnorm);
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

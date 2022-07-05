/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surfing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:08:48 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 15:09:07 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	surfing(t_poly *poly, int dotindxs[3], t_cart *dots, void *txtr)
{
	poly->vrtxs[0].dot = dotindxs[0];
	poly->vrtxs[1].dot = dotindxs[1];
	poly->vrtxs[2].dot = dotindxs[2];
	normbuilder(&dots[poly->vrtxs[0].dot], &dots[poly->vrtxs[1].dot],
		&dots[poly->vrtxs[2].dot], &poly->srcnorm);
	poly->txtr = txtr;
}

void	repairspherenormal(t_poly *poly, int dotindxs[3],
			t_cart *dots, void *txtr)
{
	t_vrtx	buf;
	float	scalar;

	surfing(poly, dotindxs, dots, txtr);
	ft_dotprod(&dots[dotindxs[0]], &poly->srcnorm, &scalar);
	if (scalar > 0)
		return ;
	buf = poly->vrtxs[2];
	poly->vrtxs[2] = poly->vrtxs[1];
	poly->vrtxs[1] = buf;
	normbuilder(&dots[poly->vrtxs[0].dot], &dots[poly->vrtxs[1].dot],
		&dots[poly->vrtxs[2].dot], &poly->srcnorm);
}

void	polarsurfing(t_cart *dots, t_poly **poly, int lttd, void *txtr)
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

void	polarjointing(t_cart *dots, t_poly *poly, void *txtr, int dotnum)
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

#include "minirt.h"

void	definepols(t_cart *dots, float radius, t_axis *rotcircle)
{
	cartbuilder(radius, 0, 0, dots);
	cartbuilder(-radius, 0, 0, dots + 1);
	if (!rotcircle)
		return ;
	quartrot(dots, rotcircle);
	quartrot(dots + 1, rotcircle);
}

int	circledotsfiller(t_cart *dots, float radius, t_axis *rotcircle, t_bool skippols)
{
	int		step;
	int		dotnum;
	t_axis	rotdot;

	step = 2 * M_PI / RNDSGMNTS;
	vectorbuilder(0, 0, 1, &rotdot);
	dotnum = -1;
	while (++dotnum < RNDSGMNTS - 2 * skippols)
	{
		cartbuilder(radius, 0, 0, dots + dotnum);
		rotdot.ang = step * (dotnum + (1 + (dotnum > (RNDSGMNTS - 2) / 2)) * skippols);
		quartrot(&dots[dotnum], &rotdot);
		if (rotcircle)
			quartrot(&dots[dotnum], rotcircle);
	}
	return (dotnum + 2 * (!skippols));
}

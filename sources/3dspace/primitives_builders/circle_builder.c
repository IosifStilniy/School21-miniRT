#include "minirt.h"

void	definepols(t_cart *dots, float radius, t_axis *rotcircle)
{
	cartbuilder(radius, 0, 0, &dots[0]);
	cartbuilder(-radius, 0, 0, &dots[1]);
	if (!rotcircle)
		return ;
	quartrot(&dots[0], rotcircle);
	quartrot(&dots[1], rotcircle);
}

int	circledotsfiller(t_cart *dots, float radius, t_axis *rotcircle, t_bool skippols)
{
	float	step;
	int		dotnum;
	t_axis	rotdot;

	step = 2 * M_PI / RNDSGMNTS;
	vectorbuilder(0, 0, 1, &rotdot);
	rotdot.ang = step * skippols;
	dotnum = -1;
	while (++dotnum < RNDSGMNTS - 2 * skippols)
	{
		cartbuilder(radius, 0, 0, &dots[dotnum]);
		quartrot(&dots[dotnum], &rotdot);
		if (rotcircle)
			quartrot(&dots[dotnum], rotcircle);
		rotdot.ang += step;
		if (skippols && dotnum + 1 == (RNDSGMNTS - 2) / 2)
			rotdot.ang += step;
	}
	return (dotnum);
}

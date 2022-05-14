#include "minirt.h"

void	definepols(t_vrtx *dots, float radius, t_axis *rotcircle)
{
	cartbuilder(radius, 0, 0, &dots->dot);
	dots->polynorms = NULL;
	cartbuilder(-radius, 0, 0, &dots[1].dot);
	dots[1].polynorms = NULL;
	if (!rotcircle)
		return ;
	quartrot(&dots->dot, rotcircle);
	quartrot(&dots[1].dot, rotcircle);
}

int	circledotsfiller(t_vrtx *dots, float radius, t_axis *rotcircle, t_bool skippols)
{
	int		step;
	int		dotnum;
	t_axis	rotdot;

	step = 2 * M_PI / RNDSGMNTS;
	vectorbuilder(0, 0, 1, &rotdot);
	dotnum = -1;
	while (++dotnum < RNDSGMNTS - 2 * skippols)
	{
		dots[dotnum].polynorms = NULL;
		cartbuilder(radius, 0, 0, &dots[dotnum].dot);
		rotdot.ang = step * (dotnum + (1 + (dotnum > (RNDSGMNTS - 2) / 2)) * skippols);
		quartrot(&dots[dotnum].dot, &rotdot);
		if (rotcircle)
			quartrot(&dots[dotnum], rotcircle);
	}
	return (dotnum + 2 * (!skippols));
}

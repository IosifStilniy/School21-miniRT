#include "minirt.h"

void	definepols(t_vrtx *dots, float radius, t_axis *rotcircle)
{
	cartbuilder(radius, 0, 0, &dots->dot);
	cartbuilder(0, 0, 0, &dots->norm);
	cartbuilder(-radius, 0, 0, &dots[1].dot);
	cartbuilder(0, 0, 0, &dots[1].norm);
	if (!rotcircle)
		return ;
	quartrot(&dots->dot, rotcircle);
	quartrot(&dots[1].dot, rotcircle);
}

int	circledotsfiller(t_vrtx *dots, float radius, t_axis *rotcircle, t_bool skippols)
{
	int		step;
	int		numstep;
	int		dotnum;
	t_axis	rotdot;

	step = 2 * M_PI / RNDSGMNTS;
	vectorbuilder(0, 0, 1, &rotdot);
	dotnum = -1;
	while (++dotnum < RNDSGMNTS - 2 * skippols)
	{
		cartbuilder(0, 0, 0, &dots[dotnum].norm);
		cartbuilder(radius, 0, 0, &dots[dotnum].dot);
		numstep = dotnum;
		if (skippols)
			numstep += 1 + (dotnum > (RNDSGMNTS - 2) / 2);
		rotdot.ang = step * numstep;
		quartrot(&dots[dotnum].dot, &rotdot);
		if (rotcircle)
			quartrot(&dots[dotnum], rotcircle);
	}
	return (dotnum + 2 * (!skippols));
}

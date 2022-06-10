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
	float	step;
	int		dotnum;
	t_axis	rotdot;

	step = 2 * M_PI / RNDSGMNTS;
	vectorbuilder(0, 0, 1, &rotdot);
	rotdot.ang = step * skippols;
	dotnum = -1;
	while (++dotnum < RNDSGMNTS - 2 * skippols)
	{
		cartbuilder(0, 0, 0, &dots[dotnum].norm);
		cartbuilder(radius, 0, 0, &dots[dotnum].dot);
		quartrot(&dots[dotnum].dot, &rotdot);
		if (rotcircle)
			quartrot(&dots[dotnum].dot, rotcircle);
		rotdot.ang += step;
		if (skippols && dotnum + 1 == (RNDSGMNTS - 2) / 2)
			rotdot.ang += step;
	}
	return (dotnum);
}

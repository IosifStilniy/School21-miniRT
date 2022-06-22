#include "minirt.h"

void	changevalue(float *param, float step, t_bool increase)
{
	if (increase)
		*param += step;
	else
		*param -= step;
}

t_bool	changecart(int arrow, t_cart *param, t_step *step)
{
	int		row;
	t_bool	increase;

	row = arrow / 2;
	increase = arrow % 2;
	if (!row)
		changevalue(&param->x, step->step, increase);
	else if (row == 1)
		changevalue(&param->y, step->step, increase);
	else if (row == 2)
		changevalue(&param->z, step->step, increase);
	else
	{
		changevalue(&step->step, step->stepping, increase);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	changecamparams(int	arrow, t_win *win, t_step *fov)
{
	int		row;
	t_bool	increase;

	row = arrow / 2;
	increase = arrow % 2;
	if (row)
	{
		changevalue(&fov->step, fov->stepping, increase);
		return (FALSE);
	}
	changevalue(&win->camera.fov, fov->step, increase);
	win->camera.focus = win->cntr.x / tanf(win->camera.fov);
	return (TRUE);
}

void	changesensetive(int arrow, t_sens *sens)
{
	int		row;
	t_bool	increase;

	row = arrow / 2;
	increase = arrow % 2;
	if (row)
		changevalue(&sens->kval, sens->move.step, increase);
	else
		changevalue(&sens->mval, sens->mouse.step, increase);
}

t_bool	changeparams(int x, int y, t_intrfc *intrfc, t_win *win)
{
	t_bool	btn;
	int		i;

	if (!intrfc->selected)
		return (FALSE);
	btn = FALSE;
	i = -1;
	while (++i < 24)
		if (inbounds(intrfc->arrows[i], x, y) && ++btn)
			break ;
	if (!btn)
		return (FALSE);
	if (i / 2 >= 8)
		return (changecart(i - 12, intrfc->selected->colrs, &intrfc->settings.color));
	else if (i / 2 >= 4)
		return (changecart(i - 4, intrfc->selected->dots.scale, &intrfc->settings.size));
	else if (i / 2 >= 2)
		return (changecamparams(i - 4, win, &intrfc->settings.fov));
	changesensetive(i, &intrfc->settings.sens);
	return (FALSE);
}

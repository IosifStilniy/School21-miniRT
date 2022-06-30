#include "minirt.h"

void	changevalue(float *param, float step, t_bool increase)
{
	if (increase)
		*param += step;
	else
		*param -= step;
}

t_bool	changecart(int arrow, t_cart *param, float bounds[2], t_step *step)
{
	int		row;
	float	*val;
	t_bool	increase;

	row = arrow / 2;
	increase = arrow % 2;
	if (!row)
		val = &param->x;
	else if (row == 1)
		val = &param->y;
	else if (row == 2)
		val = &param->z;
	else
	{
		changevalue(&step->step, step->stepping, increase);
		return (FALSE);
	}
	changevalue(val, step->step, increase);
	if (bounds && *val < bounds[0])
		*val = bounds[0];
	else if (bounds && *val > bounds[1])
		*val = bounds[1];
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
	changevalue(&win->camera->fov, fov->step, increase);
	win->camera->focus = win->cntr.x / tanf(win->camera->fov);
	cornerbuilder(win->camera->corners, &win->cntr, win->camera->focus);
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
	float	colorbounds[2];
	int		i;

	colorbounds[0] = 0;
	colorbounds[1] = 1;
	btn = FALSE;
	i = -1;
	while (++i < 24)
		if (inbounds(intrfc->arrows[i], x, y) && ++btn)
			break ;
	if (!btn)
		return (FALSE);
	if (intrfc->selected && i / 2 >= 8)
		return (changecart(i - 16, intrfc->selected->colrs, colorbounds,
			&intrfc->settings.color));
	else if (intrfc->selected && i / 2 >= 4)
		return (changecart(i - 8, intrfc->selected->dots.scale, NULL,
			&intrfc->settings.size));
	else if (i / 2 >= 2)
		return (changecamparams(i - 4, win, &intrfc->settings.fov));
	changesensetive(i, &intrfc->settings.sens);
	return (FALSE);
}

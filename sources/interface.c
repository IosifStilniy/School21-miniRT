#include "minirt.h"

t_bool	inbounds(t_button btn, int x, int y)
{
	if (!(btn.leftup.x <= x && x <= btn.bottomright.x))
		return (FALSE);
	return (btn.leftup.y <= y && y <= btn.bottomright.y);
}

void	changevalue(float *param, float step, t_bool increase)
{
	if (increase)
		*param += step;
	else
		*param -= step;
}

void	changecart(int arrow, t_cart *param, t_step *step)
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
		changevalue(&step->step, step->stepping, increase);
	return (FALSE);
}

void	changeparams(int x, int y, t_intrfc *intrfc, t_win *win)
{
	t_bool	btn;
	int		i;

	if (!intrfc->selected)
		return ;
	btn = FALSE;
	i = -1;
	while (++i < 20)
		if (inbounds(intrfc->arrows[i], x, y) && ++btn)
			break ;
	if (!btn)
		return ;
	if (i / 2 >= 6)
		changecart(i - 12, intrfc->selected->colrs, &intrfc->color);
	else if (i / 2 >= 2)
		changecart(i - 4, intrfc->selected->dots.scale, &intrfc->size);
	else if (i / 2)
		intrfc->fov.step += -intrfc->fov.stepping * (i % 2 - 1) + intrfc->fov.stepping * (i % 2);
	else
	{
		win->camera.fov += intrfc->fov.step;
		win->camera.focus = win->cntr.x / tanf(win->camera.fov);
	}
}

t_obj	*selectobject(t_list *camobjs, t_cart *vec)
{
	t_obj	*obj;
	t_cart	res;
	t_obj	*nearest;

	nearest = NULL;
	while (camobjs)
	{
		obj = camobjs->content;
		if (obj->crdstm.pos.z < 1)
		{
			camobjs = camobjs->next;
			continue ;
		}
		vectorsizing(obj->crdstm.pos.z / vec->z, vec, &res, NULL);
		objtoobjpos(&obj->crdstm.pos, &res);
		if (vectorlength(&res) > obj->outframe)
		{
			camobjs = camobjs->next;
			continue ;
		}
		if (!nearest || obj->crdstm.pos.z < nearest->crdstm.pos.z)
			nearest = obj;
	}
	return (nearest);
}

void	printvalue(float value, int x, int y, t_info *info)
{
	char	*valstr;
	char	*buf;
	char	*res;

	valstr = ft_itoa(lrintf(truncf(value)));
	buf = ft_strjoin(valstr, ".");
	free(valstr);
	value -= truncf(value);
	value *= 10;
	valstr = ft_itoa(lrintf(truncf(value)));
	res = ft_strjoin(buf, valstr);
	free(buf);
	free(valstr);
	mlx_string_put(info->mlx_ptr, info->win.win, x, y, FRAMECLR, res);
	free(res);
}

void	interfacebuilder(t_info *info)
{
	t_res	*campos;
	t_res	*objpos;

	campos = &info->interface.campos;
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->interface.cam.img, campos->x, campos->y);
	printvalue(info->win.camera.fov * 360 / M_PI, campos->x + 10, campos->y + 10, info);
	printvalue(info->interface.fov.step * 180 / M_PI, campos->x + 10, campos->y + 10, info);
	if (info->interface.selected)
		mlx_string_put(info->mlx_ptr, info->win.win, campos->x, campos->y, FRAMECLR, "TRUE");
	else
	{
		mlx_string_put(info->mlx_ptr, info->win.win, campos->x, campos->y, FRAMECLR, "FALSE");
		return ;
	}
	objpos = &info->interface.objpos;
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->interface.obj.img, objpos->x, objpos->y);
	printvalue(info->interface.selected->dots.scale->x, objpos->x + 60, objpos->y + 60, info);
	printvalue(info->interface.selected->dots.scale->y, objpos->x + 60, objpos->y + 80, info);
	printvalue(info->interface.selected->dots.scale->z, objpos->x + 60, objpos->y + 100, info);
	printvalue(info->interface.size.step, objpos->x + 60, objpos->y + 120, info);
	printvalue(info->interface.selected->colrs->x, objpos->x + 60, objpos->y + 140, info);
	printvalue(info->interface.selected->colrs->y, objpos->x + 60, objpos->y + 160, info);
	printvalue(info->interface.selected->colrs->z, objpos->x + 60, objpos->y + 180, info);
	printvalue(info->interface.color.step * 255, objpos->x + 60, objpos->y + 120, info);
}

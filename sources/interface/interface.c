#include "minirt.h"

t_bool	inbounds(t_button btn, int x, int y)
{
	if (!(btn.leftup.x <= x && x <= btn.bottomright.x))
		return (FALSE);
	return (btn.leftup.y <= y && y <= btn.bottomright.y);
}

static void	printvalue(float value, int x, int y, t_info *info)
{
	char	*valstr;
	char	*buf;
	char	*res;

	valstr = ft_itoa(lrintf(truncf(value)));
	buf = ft_strjoin(valstr, ".");
	free(valstr);
	value -= truncf(value);
	value *= 10;
	valstr = ft_itoa(lrintf(value));
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
	printvalue(info->interface.settings.sens.mval, campos->x + 63, campos->y + 45, info);
	printvalue(info->interface.settings.sens.kval, campos->x + 63, campos->y + 70, info);
	printvalue(info->win.camera.fov * 360 / M_PI, campos->x + 63, campos->y + 133, info);
	printvalue(info->interface.settings.fov.step * 360 / M_PI, campos->x + 63, campos->y + 158, info);
	if (info->win.camera.attached.obj)
		mlx_string_put(info->mlx_ptr, info->win.win, campos->x + 63, campos->y + 175, FRAMECLR, "TRUE");
	else
		mlx_string_put(info->mlx_ptr, info->win.win, campos->x + 63, campos->y + 175, FRAMECLR, "FALSE");
	if (!info->interface.selected)
		return ;
	objpos = &info->interface.objpos;
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->interface.obj.img, objpos->x, objpos->y);
	printvalue(info->interface.selected->dots.scale->x, objpos->x + 60, objpos->y + 60, info);
	printvalue(info->interface.selected->dots.scale->y, objpos->x + 60, objpos->y + 80, info);
	printvalue(info->interface.selected->dots.scale->z, objpos->x + 60, objpos->y + 100, info);
	printvalue(info->interface.settings.size.step, objpos->x + 60, objpos->y + 120, info);
	printvalue(info->interface.selected->colrs->x * 255, objpos->x + 60, objpos->y + 140, info);
	printvalue(info->interface.selected->colrs->y * 255, objpos->x + 60, objpos->y + 160, info);
	printvalue(info->interface.selected->colrs->z * 255, objpos->x + 60, objpos->y + 180, info);
	printvalue(info->interface.settings.color.step * 255, objpos->x + 60, objpos->y + 120, info);
}

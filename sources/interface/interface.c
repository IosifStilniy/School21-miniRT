/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:15:37 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 18:15:38 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_bool	inbounds(t_button btn, int x, int y)
{
	if (!(btn.leftup.x <= x && x <= btn.bottomright.x))
		return (FALSE);
	return (btn.leftup.y <= y && y <= btn.bottomright.y);
}

static void	printvalue(float value, int x, int y, t_info *info)
{
	char		*valstr;
	char		*buf;
	char		*res;
	long int	dec;
	long int	fract;

	dec = lrintf(truncf(value));
	fract = lrintf((value - truncf(value)) * 10);
	dec += (fract >= 10);
	fract -= 10 * (fract >= 10);
	valstr = ft_itoa(dec);
	buf = ft_strjoin(valstr, ".");
	free(valstr);
	valstr = ft_itoa(fract);
	res = ft_strjoin(buf, valstr);
	free(buf);
	free(valstr);
	mlx_string_put(info->mlx_ptr, info->win.win, x, y, FRAMECLR, res);
	free(res);
}

void	printcaminterface(void *mlx, t_win *win, t_intrfc *interface,
	t_info *info)
{
	mlx_put_image_to_window(mlx, win->win, interface->cam.img,
		interface->campos.x, interface->campos.y);
	printvalue(interface->settings.sens.mval, interface->campos.x + 90,
		interface->campos.y + 45, info);
	printvalue(interface->settings.sens.kval, interface->campos.x + 90,
		interface->campos.y + 68, info);
	printvalue(win->camera->fov * 360 / M_PI, interface->campos.x + 83,
		interface->campos.y + 127, info);
	printvalue(interface->settings.fov.step * 360 / M_PI,
		interface->campos.x + 83, interface->campos.y + 152, info);
	if (info->win.camera->attached.obj)
		mlx_string_put(info->mlx_ptr, info->win.win, interface->campos.x + 80,
			interface->campos.y + 172, 0x0000FF00, "TRUE");
	else
		mlx_string_put(info->mlx_ptr, info->win.win, interface->campos.x + 80,
			interface->campos.y + 172, 0x00FF0000, "FALSE");
}

void	printobjinterface(void *mlx, t_win *win, t_intrfc *interface,
	t_info *info)
{
	mlx_put_image_to_window(mlx, win->win, interface->obj.img,
		interface->objpos.x, interface->objpos.y);
	printvalue(interface->selected->dots.scale->x, interface->objpos.x + 90,
		interface->objpos.y + 50, info);
	printvalue(interface->selected->dots.scale->y, interface->objpos.x + 90,
		interface->objpos.y + 73, info);
	printvalue(interface->selected->dots.scale->z, interface->objpos.x + 90,
		interface->objpos.y + 94, info);
	printvalue(interface->settings.size.step, interface->objpos.x + 90,
		interface->objpos.y + 113, info);
	printvalue(interface->selected->colrs->x * 255, interface->objpos.x + 80,
		interface->objpos.y + 172, info);
	printvalue(interface->selected->colrs->y * 255, interface->objpos.x + 80,
		interface->objpos.y + 190, info);
	printvalue(interface->selected->colrs->z * 255, interface->objpos.x + 80,
		interface->objpos.y + 210, info);
	printvalue(interface->settings.color.step * 255, interface->objpos.x + 80,
		interface->objpos.y + 232, info);
}

void	interfacebuilder(t_info *info)
{
	t_obj	*selected;

	selected = info->interface.selected;
	if (info->interface.selected)
	{
		if (objinframe(selected, &info->win.cntr, info->win.camera->focus))
			roundselected(&selected->crdstm.pos, selected->outframe,
				&info->win, info->mlx_ptr);
		printobjinterface(info->mlx_ptr, &info->win, &info->interface, info);
	}
	printcaminterface(info->mlx_ptr, &info->win, &info->interface, info);
}

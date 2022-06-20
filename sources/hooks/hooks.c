/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/20 22:36:43 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	keydownhndlr(int keycode, t_info *info)
{
	// if (keycode == KEY_SHIFT)
	// 	info->keybrd.focus = (!info->keybrd.focus);
	keyshifting(keycode, info);
	return (0);
}

int	emptymove(int x, int y, t_info *info)
{
	(void)x;
	(void)y;
	(void)info;
	return (0);
}

int	mousemove(int x, int y, t_info *info)
{
	camrotating(&info->win.camera, info, x, y);
	ft_draw_screen(info);
	mlx_mouse_move(info->win.win, 0, info->mouse.yshift);
	return (0);
}

int	btnup(int btn, int x, int y, t_info *info)
{
	t_axis	vec;

	if (btn != LMB)
		return (0);
	vectorbuilder(x, y, info->win.camera.focus, &vec);
	if (!(info->interface.frame.x <= x && x <= info->win.res.x))
		info->interface.selected = selectobject(info->win.camera.objs, &vec.vector);
	else if (!(0 <= y && y <= info->interface.frame.y))
		info->interface.selected = selectobject(info->win.camera.objs, &vec.vector);
	else
		changeparams();
	ft_draw_screen(info);
	return (0);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_I)
		info->keybrd.interface = (!info->keybrd.interface);
	else if (keycode == KEY_ESC)
		exit(0);
	mlx_hook(info->win.win, 6, 1L << 6, &mousemove, info);
	if (!info->keybrd.interface)
		return (0);
	mlx_hook(info->win.win, 4, 1L << 2, &btnpress, info);
	mlx_hook(info->win.win, 5, 1L << 3, &btnup, info);
	mlx_hook(info->win.win, 6, 1L << 6, &emptymove, info);
	return (0);
}

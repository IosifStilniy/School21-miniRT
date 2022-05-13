/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/12 20:58:56 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	keydownhndlr(int keycode, t_info *info)
{
	if (keycode == K_CMND)
		info->keybrd.zrot = (!info->keybrd.zrot);
	else if (keycode == K_SHIFT)
		info->keybrd.zoom = (!info->keybrd.zoom);
	else if (keycode == K_CNTRL)
		info->keybrd.focus = (!info->keybrd.focus);
	keyshifting(keycode, info);
	keyrotating(keycode, info);
	return (0);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_CMND)
		info->keybrd.zrot = (!info->keybrd.zrot);
	else if (keycode == KEY_SHIFT)
		info->keybrd.zoom = (!info->keybrd.zoom);
	else if (keycode == KEY_CNTRL)
		info->keybrd.focus = (!info->keybrd.focus);
	else if (keycode == KEY_L)
		info->keybrd.legend = (!info->keybrd.legend);
	else if (keycode == KEY_C)
		info->keybrd.movecam = (!info->keybrd.movecam);
	else if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int	btnpress(int btn, int x, int y, t_info *info)
{
	info->mouse.pos.x = x - info->img->shift.crdstm.x;
	info->mouse.pos.y = y - info->img->shift.crdstm.y;
	if (btn == LMB)
	{
		info->mouse.pos.x -= info->win.cntr.x;
		info->mouse.pos.y -= info->win.cntr.y;
		info->mouse.rot = (!info->mouse.rot);
		vectorbuilder(info->mouse.pos.x, info->mouse.pos.y,
			info->win->view.dstnc, &info->mouse.vpos.v1);
		if (info->keybrd.zrot)
			vectorbuilder(info->mouse.pos.x, info->mouse.pos.y, 0,
				&info->mouse.vpos.v1);
	}
	if (btn == MMB)
		info->mouse.shift = (!info->mouse.shift);
	if (btn == SCRL_UP || btn == SCRL_DOWN)
		scrolling(btn, info);
	return (0);
}

int	btnup(int btn, int x, int y, t_info *info)
{
	x++;
	y++;
	if (btn == MMB)
		info->mouse.shift = (!info->mouse.shift);
	if (btn == LMB)
		info->mouse.rot = (!info->mouse.rot);
	return (0);
}

int	mousemove(int x, int y, t_info *info)
{
	if (!info->keybrd.zoom && !info->keybrd.focus && info->mouse.shift)
		mouseshifting(info, x, y);
	else if (info->mouse.rot)
	{
		x -= info->img->shift.crdstm.x + info->win->cntr.x;
		y -= info->img->shift.crdstm.y + info->win->cntr.y;
		mouserotating(info, x, y);
	}
	else if ((info->keybrd.zoom || info->keybrd.focus) && info->mouse.shift)
		mousezooming(info, y);
	return (0);
}

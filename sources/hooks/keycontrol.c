/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/08 22:43:24 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keyshifting(int keycode, t_info *info)
{
	t_cart	camdir;
	t_cart	objsdir;
	t_cart	a;
	t_cart	b;
	t_axis	c;

	cartbuilder((keycode == KEY_A) - (keycode == KEY_D),
		(keycode == KEY_SPACE) - (keycode == KEY_SHIFT),
		(keycode == KEY_S) - (keycode == KEY_W),
		&objsdir);
	if (!(objsdir.x + objsdir.y + objsdir.z))
		return ;
	if (objsdir.x)
		cartcopy(&info->win.camera.crdstm.ox.vector, &camdir, 1);
	else if (objsdir.y)
		cartcopy(&info->win.camera.crdstm.oy.vector, &camdir, 1);
	else if (objsdir.z)
		cartcopy(&info->win.camera.crdstm.oz.vector, &camdir, 1);
	camshifting(&info->win.camera, &camdir, &objsdir, SHIFT_SPEED);
	a = objcast(info->win.camera.camobjs.objs)->dots.pos->dot;
	b = objcast(info->win.camera.camobjs.objs)->crdstm.pos;
	vectorbuilder(a.x - b.x, a.y - b.y, a.z - b.z, &c);
	// printf("lng: %.3f dot1: %.3f %.3f %.3f\n", c.length, a.x, a.y, a.z);
	createview(&info->win.camera);
	// imgdefiner(info->img, info->win, info->mlx);
	// paintpic(info->dots, info->img, info->win, info->mlx);
}

// void	scrolling(int btn, t_info *info)
// {
// 	if (btn == SCRL_UP && info->win->view.scale.cur > 0.9 * info->win->res.y)
// 		return ;
// 	info->win->view.scale.old = info->win->view.scale.cur;
// 	if (!info->win->view.scale.cur && btn == SCRL_UP)
// 		info->win->view.scale.cur = 0.1;
// 	info->win->view.scale.cur *= 1 - 0.1 * (btn != SCRL_UP)
// 		+ 0.1 * (btn == SCRL_UP);
// 	info->win->view.scale.cur *= (info->win->view.scale.cur > 0);
// 	engine(info->win, info->dots);
// 	imgdefiner(info->img, info->win, info->mlx);
// 	paintpic(info->dots, info->img, info->win, info->mlx);
// }

int	finish(void)
{
	exit(0);
}

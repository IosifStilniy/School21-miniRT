/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/23 19:22:38 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keydirbuilder(int keycode, t_cart *dir)
{
	dir->x = (keycode == KEY_D) - (keycode == KEY_A);
	dir->y = (keycode == KEY_SHIFT) - (keycode == KEY_SPACE);
	dir->z = (keycode == KEY_W) - (keycode == KEY_S);
}

void	keyaxisbuilder(int keycode, t_cart *axis)
{
	axis->x = (keycode == KEY_UP) - (keycode == KEY_DOWN);
	axis->y = (keycode == KEY_RIGHT) - (keycode == KEY_LEFT);
	axis->z = (keycode == KEY_E) - (keycode == KEY_Q);
}

void	keyshifting(t_cart *dir, t_cart *axis, t_info *info)
{
	t_axis	res;

	if (dir->x + dir->y + dir->z)
	{
		camshifting(&info->win.camera, info, dir, SHIFT_SPEED * info->interface.settings.sens.kval);
		return ;
	}
	res.ang = DEFANG * M_PI / 180;
	dotcrdstmtrnsltn(axis, &res.vector, NULL, &info->win.camera.crdstm);
	if (info->win.camera.attached.obj)
	{
		crdstmrotbyaxis(&info->win.camera.attached.obj->crdstm, &res, NULL);
		camfromobjcrdstm(&info->win.camera.crdstm, &info->win.camera.attached);
	}
	else
		crdstmrotbyaxis(&info->win.camera.crdstm, &res, NULL);
	initview(info->objects, &info->win.camera, &info->lights);
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

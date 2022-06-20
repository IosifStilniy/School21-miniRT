/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/20 18:55:51 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keyshifting(int keycode, t_info *info)
{
	t_cart	objsdir;
	t_cart	axis;
	t_axis	res;

	objsdir.x = (keycode == KEY_A) - (keycode == KEY_D);
	objsdir.y = (keycode == KEY_SPACE) - (keycode == KEY_SHIFT);
	objsdir.z = (keycode == KEY_S) - (keycode == KEY_W);
	axis.x = (keycode == KEY_UP) - (keycode == KEY_DOWN);
	axis.y = (keycode == KEY_RIGHT) - (keycode == KEY_LEFT);
	axis.z = (keycode == KEY_E) - (keycode == KEY_Q);
	if (axis.x + axis.y + axis.z)
	{
		res.ang = DEFANG * M_PI / 180;
		dotcrdstmtrnsltn(&axis, &res.vector, NULL, &info->win.camera.crdstm);
		crdstmrotbyaxis(&info->win.camera.crdstm, &res, NULL);
		initview(info->objects, &info->win.camera, &info->lights);
	}
	else if (objsdir.x + objsdir.y + objsdir.z)
		camshifting(&info->win.camera, info, &objsdir, SHIFT_SPEED);
	else if (keycode == KEY_R)
		info->keybrd.render = (!info->keybrd.render);
	ft_draw_screen(info);
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

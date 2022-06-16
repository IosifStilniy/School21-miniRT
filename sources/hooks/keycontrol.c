/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/16 21:32:25 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	rolling(t_axis *axis, t_list *camobjs, t_crdstm *camera)
{
	t_obj	*obj;
	t_axis	worldaxis;

	axis->ang = DEFANG * M_PI / 180;
	while (camobjs)
	{
		obj = camobjs->content;
		crdstmrotbyaxis(&obj->crdstm, axis, NULL);
		quartrot(&obj->crdstm.pos, axis);
		engine(&obj->dots, &obj->polys, &obj->crdstm);
		camobjs = camobjs->next;
	}
	worldaxis.ang = DEFANG * M_PI / 180;
	cartcopy(&camera->oz.vector, &worldaxis.vector, 1);
	if (axis->vector.z > 0)
		negativevector(&worldaxis.vector);
	crdstmrotbyaxis(camera, &worldaxis, NULL);
}

void	keyshifting(int keycode, t_info *info)
{
	t_cart	objsdir;
	t_axis	axis;

	cartbuilder((keycode == KEY_A) - (keycode == KEY_D),
		(keycode == KEY_SPACE) - (keycode == KEY_SHIFT),
		(keycode == KEY_S) - (keycode == KEY_W),
		&objsdir);
	vectorbuilder(0, 0, (keycode == KEY_Q) - (keycode == KEY_E), &axis);
	if (axis.vector.z)
		rolling(&axis, info->win.camera.objs, &info->win.camera.crdstm);
	if (objsdir.x + objsdir.y + objsdir.z)
	{
		dottranslation(&info->win.camera.lightpos, &objsdir, SHIFT_SPEED);
		camshifting(&info->win.camera.crdstm, info->win.camera.objs, &objsdir, SHIFT_SPEED);
	}
	if (keycode == KEY_R)
		info->keybrd.render = (!info->keybrd.render);
	if (info->keybrd.render)
		ft_draw_screen(info);
	else
		framepic(&info->win, info->win.camera.objs, &info->data, info->mlx_ptr);
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

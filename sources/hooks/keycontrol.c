/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/09 22:48:03 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keyshifting(int keycode, t_info *info)
{
	t_cart	camdir;
	t_cart	objsdir;

	cartbuilder((keycode == KEY_A) - (keycode == KEY_D),
		(keycode == KEY_SPACE) - (keycode == KEY_SHIFT),
		(keycode == KEY_S) - (keycode == KEY_W),
		&objsdir);
	if (!(objsdir.x + objsdir.y + objsdir.z))
		return ;
	printf("dir: %.3f %.3f %.3f\n", objsdir.x, objsdir.y, objsdir.z);
	if (objsdir.x)
		cartcopy(&info->win.camera.crdstm.ox.vector, &camdir, 1);
	else if (objsdir.y)
		cartcopy(&info->win.camera.crdstm.oy.vector, &camdir, 1);
	else if (objsdir.z)
		cartcopy(&info->win.camera.crdstm.oz.vector, &camdir, 1);
	dotstranslation(&info->lights.pos, 1, &objsdir, SHIFT_SPEED);
	camshifting(&info->win.camera, &camdir, &objsdir, SHIFT_SPEED);
	t_obj	*obj;

	obj = info->win.camera.camobjs.objs->content;
	printf("crdstm: %.3f %.3f %.3f\n", obj->crdstm.pos.x, obj->crdstm.pos.y, obj->crdstm.pos.z);
	printf("dot1: %.3f %.3f %.3f\n", obj->dots.pos->dot.x, obj->dots.pos->dot.y, obj->dots.pos->dot.z);
	printf("rad: %.3f\n", obj->outframe);
	ft_draw_screen(info);
	// t_list *crsr;
	// t_obj *obj;

	// crsr = info->win.camera.camobjs.objs;
	// while (crsr)
	// {
	// 	obj = crsr->content;
	// 	printf("obj: %.3f %.3f %.3f\n", obj->crdstm.pos.x, obj->crdstm.pos.y, obj->crdstm.pos.z);
	// 	crsr = crsr->next;
	// }
	
	// createview(&info->win.camera);
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

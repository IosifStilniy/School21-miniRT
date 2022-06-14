/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mousecontrol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:06 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/14 18:51:39 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	camrotating(t_camera *camera, void *win, int x, int y)
{
	t_axis	curpos;
	t_cart	oz;
	t_axis	axis;
	t_list	*crsr;
	t_obj	*obj;

	cartbuilder(x, -y, 4000, &curpos.vector);
	vectorsizing(1, &curpos.vector, &curpos.vector, NULL);
	cartbuilder(0, 0, 1, &oz);
	axisbuilder(&oz, &curpos.vector, &axis);
	if (comparef(axis.ang, 0, 0.5 * M_PI / 180))
		return ;
	mlx_mouse_move(win, 0, 270);
	dotcrdstmtrnsltn(&axis.vector, &curpos.vector, 1, &camera->crdstm);
	crdstmrotbyaxis(&camera->crdstm, &curpos, NULL);
	negativevector(&axis.vector);
	quartrot(&camera->lightpos, &axis);
	crsr = camera->objs;
	while (crsr)
	{
		obj = crsr->content;
		crdstmrotbyaxis(&obj->crdstm, &axis, NULL);
		quartrot(&obj->crdstm.pos, &axis);
		engine(&obj->dots, &obj->polys, &obj->crdstm);
		crsr = crsr->next;
	}
}

// void	mouserotating(t_info *info, int x, int y)
// {
// 	vectorbuilder(x, y, info->win->view.dstnc, &info->mouse.vpos.v2);
// 	if (info->keybrd.zrot)
// 		vectorbuilder(x, y, 0, &info->mouse.vpos.v2);
// 	axisbuilder(&info->mouse.vpos.v2, &info->mouse.vpos.v1,
// 		&info->win->view.axis);
// 	if (info->keybrd.zrot)
// 		axisbuilder(&info->mouse.vpos.v1, &info->mouse.vpos.v2,
// 			&info->win->view.axis);
// 	info->win->view.axis.ang *= 3;
// 	engine(info->win, info->dots);
// 	imgdefiner(info->img, info->win, info->mlx);
// 	paintpic(info->dots, info->img, info->win, info->mlx);
// 	vectorbuilder(info->mouse.vpos.v2.vector.x, info->mouse.vpos.v2.vector.y,
// 		info->mouse.vpos.v2.vector.z, &info->mouse.vpos.v1);
// }

// void	mouseshifting(t_info *info, int x, int y)
// {
// 	engine(info->win, info->dots);
// 	imgdefiner(info->img, info->win, info->mlx);
// 	paintpic(info->dots, info->img, info->win, info->mlx);
// 	info->img->shift.crdstm.x = x - info->mouse.pos.x;
// 	info->img->shift.crdstm.y = y - info->mouse.pos.y;
// }

// void	mousezooming(t_info *info, int y)
// {
// 	if (info->keybrd.zoom)
// 	{
// 		info->win->view.scale.old = info->win->view.scale.cur;
// 		info->win->view.scale.cur += info->mouse.pos.y - y
// 			+ info->img->shift.crdstm.y;
// 	}
// 	if (info->keybrd.focus)
// 		info->win->view.focus += info->mouse.pos.y - y
// 			+ info->img->shift.crdstm.y;
// 	info->win->view.scale.cur *= (info->win->view.scale.cur > 0);
// 	info->win->view.focus *= (info->win->view.focus > 0);
// 	engine(info->win, info->dots);
// 	imgdefiner(info->img, info->win, info->mlx);
// 	paintpic(info->dots, info->img, info->win, info->mlx);
// 	info->mouse.pos.y = y - info->img->shift.crdstm.y;
// }

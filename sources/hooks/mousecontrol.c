/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mousecontrol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:06 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/09 00:22:02 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	lngtg(t_cart *v)
{
	return (sqrtf(powf(v->x, 2) + powf(v->y, 2) + powf(v->z, 2)));
}

void	camrotating(t_camera *camera, void *win, int x, int y)
{
	t_axis	curpos;
	t_cart	oz;
	t_axis	axis;
	t_list	*crsr;
	t_obj	*obj;

	cartbuilder(x, y, 100, &curpos.vector);
	vectorsizing(1, &curpos.vector, &curpos.vector, NULL);
	cartbuilder(0, 0, 1, &oz);
	axisbuilder(&oz, &curpos.vector, &axis);
	if (comparef(axis.ang, 0, 0.5 * M_PI / 180))
		return ;
	mlx_mouse_move(win, 0, 940);
	gettranskoef(&camera->crdstm, &oz);
	dotcrdstmtrnsltn(&axis.vector, &curpos.vector, 1, &oz);
	crdstmrotbyaxis(&camera->crdstm, &curpos, NULL);
	negativevector(&axis.vector);
	crsr = camera->camobjs.objs;
	while (crsr)
	{
		obj = objcast(crsr);
		printf("do povorota:\n");
		printf("ox: %.3f %.3f %.3f %.3f\n", obj->crdstm.ox.vector.x, obj->crdstm.ox.vector.y, obj->crdstm.ox.vector.z, lngtg(&obj->crdstm.ox.vector));
		printf("oy: %.3f %.3f %.3f %.3f\n", obj->crdstm.oy.vector.x, obj->crdstm.oy.vector.y, obj->crdstm.oy.vector.z, lngtg(&obj->crdstm.ox.vector));
		printf("oz: %.3f %.3f %.3f %.3f\n", obj->crdstm.oz.vector.x, obj->crdstm.oz.vector.y, obj->crdstm.oz.vector.z, lngtg(&obj->crdstm.ox.vector));
		crdstmrotbyaxis(&obj->crdstm, &axis, NULL);
		printf("posle povorota:\n");
		printf("ox: %.3f %.3f %.3f %.3f\n", obj->crdstm.ox.vector.x, obj->crdstm.ox.vector.y, obj->crdstm.ox.vector.z, lngtg(&obj->crdstm.ox.vector));
		printf("oy: %.3f %.3f %.3f %.3f\n", obj->crdstm.oy.vector.x, obj->crdstm.oy.vector.y, obj->crdstm.oy.vector.z, lngtg(&obj->crdstm.ox.vector));
		printf("oz: %.3f %.3f %.3f %.3f\n", obj->crdstm.oz.vector.x, obj->crdstm.oz.vector.y, obj->crdstm.oz.vector.z, lngtg(&obj->crdstm.ox.vector));
		t_axis v;
		printf("angles:\n");
		axisbuilder(&obj->crdstm.ox.vector, &obj->crdstm.oy.vector, &v);
		printf("ox oy: %.3f\n", v.ang * 180 / M_PI);
		axisbuilder(&obj->crdstm.ox.vector, &obj->crdstm.oz.vector, &v);
		printf("ox oz: %.3f\n", v.ang * 180 / M_PI);
		axisbuilder(&obj->crdstm.oy.vector, &obj->crdstm.oz.vector, &v);
		printf("oy oz: %.3f\n", v.ang * 180 / M_PI);
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

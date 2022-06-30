/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/30 21:02:46 by dcelsa           ###   ########.fr       */
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
		camshifting(info->win.camera, info, dir, SHIFT_SPEED * info->interface.settings.sens.kval);
		return ;
	}
	res.ang = DEFANG * M_PI / 180;
	dotcrdstmtrnsltn(axis, &res.vector, NULL, &info->win.camera->crdstm);
	if (info->win.camera->attached.obj)
	{
		crdstmrotbyaxis(&info->win.camera->attached.obj->crdstm, &res, NULL);
		camfromobjcrdstm(&info->win.camera->crdstm, &info->win.camera->attached);
	}
	else
		crdstmrotbyaxis(&info->win.camera->crdstm, &res, NULL);
	initview(info->objects, info->win.camera, info->lights);
}

void	rotateattached(t_cart *dir, t_axis *axis, t_info *info)
{
	if (!info->win.camera->attached.obj)
		return ;
	if (dir->x + dir->y + dir->z)
		vectodot(&info->win.camera->attached.crdstm.pos, dir);
	else
	{
		axis->ang = M_PI_2;
		quartrot(&info->win.camera->attached.crdstm.pos, axis);
		crdstmrotbyaxis(&info->win.camera->attached.crdstm, axis, NULL);
	}
	camfromobjcrdstm(&info->win.camera->crdstm, &info->win.camera->attached);
	initview(info->objects, info->win.camera, info->lights);
}

int	finish(void)
{
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/10 21:37:36 by ncarob           ###   ########.fr       */
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
	if (objsdir.x)
		cartcopy(&info->win.camera.crdstm.ox.vector, &camdir, 1);
	else if (objsdir.y)
		cartcopy(&info->win.camera.crdstm.oy.vector, &camdir, 1);
	else if (objsdir.z)
		cartcopy(&info->win.camera.crdstm.oz.vector, &camdir, 1);
	dotstranslation(&info->lights.pos, 1, &objsdir, SHIFT_SPEED);
	camshifting(&info->win.camera, &camdir, &objsdir, SHIFT_SPEED);
	ft_draw_screen(info);
}

int	finish(void)
{
	exit(0);
}

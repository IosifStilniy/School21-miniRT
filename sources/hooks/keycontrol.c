/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycontrol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:43:02 by dcelsa            #+#    #+#             */
/*   Updated: 2022/05/12 20:56:36 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	keyshifting(int keycode, t_info *info)
{
	t_bool	shift;

	shift = FALSE;
	if (keycode == K_W && ++shift)
		info->img->shift.crdstm.y -= SHIFT_SPEED;
	if (keycode == K_S && ++shift)
		info->img->shift.crdstm.y += SHIFT_SPEED;
	if (keycode == K_A && ++shift)
		info->img->shift.crdstm.x -= SHIFT_SPEED;
	if (keycode == K_D && ++shift)
		info->img->shift.crdstm.x += SHIFT_SPEED;
	if (!shift)
		return ;
	engine(info->win, info->dots);
	imgdefiner(info->img, info->win, info->mlx);
	paintpic(info->dots, info->img, info->win, info->mlx);
}

static t_bool	rothndlr(int keycode, t_win *win, t_keybrd *keybrd, t_dot *dots)
{
	t_bool	rot;

	rot = FALSE;
	if (keycode == K_UP && ++rot)
		vectorbuilder(-1, 0, 0, &win->view.axis);
	if (keycode == K_DOWN && ++rot)
		vectorbuilder(1, 0, 0, &win->view.axis);
	if (keycode == K_LEFT && !keybrd->zrot && ++rot)
		vectorbuilder(0, 1, 0, &win->view.axis);
	if (keycode == K_RIGHT && !keybrd->zrot && ++rot)
		vectorbuilder(0, -1, 0, &win->view.axis);
	if (keycode == K_LEFT && keybrd->zrot && ++rot)
		vectorbuilder(0, 0, -1, &win->view.axis);
	if (keycode == K_RIGHT && keybrd->zrot && ++rot)
		vectorbuilder(0, 0, 1, &win->view.axis);
	if (keycode == K_Q && ++rot)
		vectorbuilder(dots->norm.vector.x, -dots->norm.vector.y,
			-dots->norm.vector.z, &win->view.axis);
	if (keycode == K_E && ++rot)
		vectorbuilder(dots->norm.vector.x, dots->norm.vector.y,
			dots->norm.vector.z, &win->view.axis);
	return (rot);
}

void	keyrotating(int keycode, t_info *info)
{
	if (!rothndlr(keycode, info->win, &info->keybrd, info->dots))
		return ;
	engine(info->win, info->dots);
	imgdefiner(info->img, info->win, info->mlx);
	paintpic(info->dots, info->img, info->win, info->mlx);
}

void	scrolling(int btn, t_info *info)
{
	if (btn == SCRL_UP && info->win->view.scale.cur > 0.9 * info->win->res.y)
		return ;
	info->win->view.scale.old = info->win->view.scale.cur;
	if (!info->win->view.scale.cur && btn == SCRL_UP)
		info->win->view.scale.cur = 0.1;
	info->win->view.scale.cur *= 1 - 0.1 * (btn != SCRL_UP)
		+ 0.1 * (btn == SCRL_UP);
	info->win->view.scale.cur *= (info->win->view.scale.cur > 0);
	engine(info->win, info->dots);
	imgdefiner(info->img, info->win, info->mlx);
	paintpic(info->dots, info->img, info->win, info->mlx);
}

int	finish(void)
{
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/21 22:41:04 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	keydownhndlr(int keycode, t_info *info)
{
	if (keycode == KEY_R)
		info->keybrd.render = !info->keybrd.render;
	else
		keyshifting(keycode, info);
	ft_draw_screen(info);
	return (0);
}

int	rotateattached(int keycode, t_info *info)
{
	t_cart	dir;
	t_axis	axis;

	if (!info->win.camera.attached.obj)
		return (0);
	keydirbuilder(keycode, &dir);
	keyaxisbuilder(keycode, &axis.vector);
	if (dir.x + dir.y + dir.z)
		vectodot(&info->win.camera.attached.crdstm.pos, &dir, FALSE);
	else if (axis.vector.x + axis.vector.y + axis.vector.z)
	{
		axis.ang = M_PI_2;
		quartrot(&info->win.camera.attached.crdstm.pos, &axis);
		crdstmrotbyaxis(&info->win.camera.attached.crdstm, &axis, NULL);
	}
	else
		return (0);
	camfromobjcrdstm(&info->win.camera.crdstm, &info->win.camera.attached);
	initview(info->objects, &info->win.camera, &info->lights);
	return (0);
}

int	emptymove(int x, int y, t_info *info)
{
	(void)x;
	(void)y;
	(void)info;
	return (0);
}

int	mousemove(int x, int y, t_info *info)
{
	camrotating(&info->win.camera, info, x, y);
	ft_draw_screen(info);
	mlx_mouse_move(info->win.win, 0, info->mouse.yshift);
	return (0);
}

void	attachobj(t_camera *camera, t_obj *obj)
{
	if (!obj || camera->attached.obj == obj)
	{
		camera->attached.obj = NULL;
		return ;
	}
	camera->attached.obj = obj;
	camera->attached.crdstm.pos.x = 0;
	camera->attached.crdstm.pos.y = -1.3 * obj->outframe;
	camera->attached.crdstm.pos.z = -1.3 * obj->outframe;
	vectorbuilder(0, 0, 1, &camera->attached.crdstm.oz);
	crdstmdefiner(&camera->attached.crdstm);
	camfromobjcrdstm(&camera->crdstm, &camera->attached);
}

int	btnup(int btn, int x, int y, t_info *info)
{
	t_axis	vec;

	if (btn != LMB)
		return (0);
	vectorbuilder(x, y, info->win.camera.focus, &vec);
	if (!(info->interface.frame.x <= x && x <= info->win.res.x))
		info->interface.selected = selectobject(info->win.camera.objs, &vec.vector);
	else if (!(0 <= y && y <= info->interface.frame.y))
		info->interface.selected = selectobject(info->win.camera.objs, &vec.vector);
	else if (inbounds(info->interface.attach, x, y) && info->interface.selected)
	{
		attachobj(&info->win.camera, info->interface.selected);
		initview(info->objects, &info->win.camera, &info->lights);
		ft_draw_screen(info);
	}
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
	if (info->interface.selected)
		roundselected(&info->interface.selected->crdstm.pos, info->interface.selected->outframe, &info->win, info->mlx_ptr);
	interfacebuilder(info);
	return (0);
}

int	btnpress(int btn, int x, int y, t_info *info)
{
	t_bool	reprint;

	if (btn != LMB)
		return (0);
	if (changeparams(x, y, &info->interface, &info->win))
		ft_draw_screen(info);
	else
		mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
	if (info->interface.selected)
		roundselected(&info->interface.selected->crdstm.pos, info->interface.selected->outframe, &info->win, info->mlx_ptr);
	interfacebuilder(info);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_I)
		info->keybrd.interface = (!info->keybrd.interface);
	else if (keycode == KEY_ESC)
		exit(0);
	mlx_hook(info->win.win, 2, 1L, &keydownhndlr, &info);
	mlx_hook(info->win.win, 6, 1L << 6, &mousemove, info);
	if (!info->keybrd.interface)
		return (0);
	mlx_hook(info->win.win, 2, 1L, &rotateattached, &info);
	mlx_hook(info->win.win, 4, 1L << 2, &btnpress, info);
	mlx_hook(info->win.win, 5, 1L << 3, &btnup, info);
	mlx_hook(info->win.win, 6, 1L << 6, &emptymove, info);
	return (0);
}

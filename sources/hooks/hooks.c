/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/22 22:34:41 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	rotateattached(t_cart *dir, t_axis *axis, t_info *info)
{
	if (!info->win.camera.attached.obj)
		return ;
	if (dir->x + dir->y + dir->z)
		vectodot(&info->win.camera.attached.crdstm.pos, dir, FALSE);
	else
	{
		axis->ang = M_PI_2;
		quartrot(&info->win.camera.attached.crdstm.pos, axis);
		crdstmrotbyaxis(&info->win.camera.attached.crdstm, axis, NULL);
	}
	camfromobjcrdstm(&info->win.camera.crdstm, &info->win.camera.attached);
	initview(info->objects, &info->win.camera, &info->lights);
}

int	keydownhndlr(int keycode, t_info *info)
{
	t_cart	dir;
	t_axis	axis;
	t_bool	movement;

	keydirbuilder(keycode, &dir);
	keyaxisbuilder(keycode, &axis.vector);
	movement = lrintf(dir.x + dir.y + dir.z + axis.vector.x + axis.vector.y + axis.vector.z);
	if (movement && !info->keybrd.interface)
		keyshifting(&dir, &axis.vector, info);
	else if (movement && info->keybrd.interface)
		rotateattached(&dir, &axis, info);
	else
		return (0);
	ft_draw_screen(info);
	return (0);
}

int	mousemove(int x, int y, t_info *info)
{
	if (info->keybrd.interface)
		return (0);
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

	if (btn != LMB || !info->keybrd.interface)
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
	if (btn != LMB || !info->keybrd.interface)
		return (0);
	if (changeparams(x, y, &info->interface, &info->win))
		ft_draw_screen(info);
	else
		mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
	if (info->interface.selected)
		roundselected(&info->interface.selected->crdstm.pos, info->interface.selected->outframe, &info->win, info->mlx_ptr);
	interfacebuilder(info);
	return (0);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_I)
		info->keybrd.interface = (!info->keybrd.interface);
	else if (keycode == KEY_R)
		info->keybrd.render = !info->keybrd.render;
	else if (keycode == KEY_ESC)
		exit(0);
	else
		return (0);
	ft_draw_screen(info);
	return (0);
}

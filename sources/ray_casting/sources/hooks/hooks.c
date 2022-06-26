/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/06/25 18:02:24 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	if (info->keybrd.interface)
		interfacebuilder(info);
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

t_bool	attachobj(t_camera *camera, t_obj *camobj, t_list *objs)
{
	if (!camobj || camera->attached.obj == camobj || !camobj->dots.dotsnum)
	{
		camera->attached.obj = NULL;
		return (FALSE);
	}
	while (objs && objcast(objs)->dots.dots != camobj->dots.dots)
		objs = objs->next;
	if (!objs)
		return (FALSE);
	camera->attached.obj = objs->content;
	camera->attached.crdstm.pos.x = 0;
	camera->attached.crdstm.pos.y = -1.3 * camobj->outframe;
	camera->attached.crdstm.pos.z = -1.3 * camobj->outframe;
	vectorbuilder(0, 0, 1, &camera->attached.crdstm.oz);
	crdstmdefiner(&camera->attached.crdstm);
	camfromobjcrdstm(&camera->crdstm, &camera->attached);
	return (TRUE);
}

int	btnup(int btn, int x, int y, t_info *info)
{
	t_axis	vec;
	t_bool	refresh;

	if (btn != LMB || !info->keybrd.interface)
		return (0);
	vectorbuilder(x - info->win.cntr.x, y - info->win.cntr.y, info->win.camera.focus, &vec);
	refresh = changeparams(x, y, &info->interface, &info->win);
	if (!(info->interface.frame.x <= x && x <= info->win.res.x)
		|| !(0 <= y && y <= info->interface.frame.y))
		info->interface.selected = selectobject(info->win.camera.objs, &vec.vector);
	else if (inbounds(info->interface.attach, x, y) && attachobj(&info->win.camera, info->interface.selected, info->objects) && ++refresh)
		info->interface.selected = NULL;
	if (refresh)
		initview(info->objects, &info->win.camera, &info->lights);
	if (refresh)
		ft_draw_screen(info);
	if (!refresh)
		mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
	interfacebuilder(info);
	return (0);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_I)
	{
		mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
		info->keybrd.interface = (!info->keybrd.interface);
		if (info->keybrd.interface)
		{
			interfacebuilder(info);
			mlx_mouse_show();
			return (0);
		}
		info->interface.selected = NULL;
		mlx_mouse_hide();
		mlx_mouse_move(info->win.win, 0, info->mouse.yshift);
	}
	else if (keycode == KEY_R)
	{
		info->keybrd.render = !info->keybrd.render;
		ft_draw_screen(info);
		if (info->keybrd.interface && !info->keybrd.render)
			interfacebuilder(info);
	}
	else if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

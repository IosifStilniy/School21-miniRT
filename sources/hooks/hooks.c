/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 02:40:40 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 18:20:08 by dcelsa           ###   ########.fr       */
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
	movement = lrintf(dir.x + dir.y + dir.z + axis.vector.x + axis.vector.y
			+ axis.vector.z);
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

void	interfacehandler(t_info *info)
{
	mlx_put_image_to_window(info->mlx_ptr, info->win.win, info->data.img, 0, 0);
	info->keybrd.interface = (!info->keybrd.interface);
	if (info->keybrd.interface)
	{
		interfacebuilder(info);
		mlx_mouse_show();
		return ;
	}
	info->interface.selected = NULL;
	mlx_mouse_hide();
	mlx_mouse_move(info->win.win, 0, info->mouse.yshift);
}

char	*changecamtxt(int current, int count)
{
	char	*overal;
	char	*num;
	char	*buf;

	num = ft_itoa(current);
	buf = ft_strjoin("Camera: ", num);
	free(num);
	num = ft_itoa(count);
	overal = ft_strjoin(buf, " of ");
	free(buf);
	buf = ft_strjoin(overal, num);
	free(num);
	free(overal);
	return (buf);
}

void	switchcam(t_camera **camera, t_list *cameras, int keycode,
	char **camtxt)
{
	int		camcount;
	int		i;

	if (keycode == KEY_OPBRCT && *camera == cameras->content)
		return ;
	if (keycode == KEY_CLBRCT && *camera == ft_lstlast(cameras)->content)
		return ;
	camcount = ft_lstsize(cameras);
	i = 0;
	while (cameras && ++i)
	{
		if (keycode == KEY_OPBRCT && cameras->next->content == *camera)
			*camera = cameras->content;
		else if (keycode == KEY_CLBRCT && cameras->content == *camera)
			*camera = cameras->next->content;
		cameras = cameras->next;
	}
	*camtxt = changecamtxt(i, camcount);
}

int	keyuphndlr(int keycode, t_info *info)
{
	if (keycode == KEY_I)
		interfacehandler(info);
	else if (keycode == KEY_R || keycode == KEY_N)
	{
		if (keycode == KEY_R)
			info->keybrd.render = !info->keybrd.render;
		else if (keycode == KEY_N)
			info->keybrd.normalpaint = (!info->keybrd.normalpaint);
		ft_draw_screen(info);
		if (info->keybrd.interface && !info->keybrd.render)
			interfacebuilder(info);
	}
	else if (keycode == KEY_OPBRCT || keycode == KEY_CLBRCT)
		switchcam(&info->win.camera, info->win.cameras, keycode,
			&info->camtext);
	else if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

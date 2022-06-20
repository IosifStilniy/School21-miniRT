/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/20 22:32:59 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	wininit(t_info *info, char *prog, char *file)
{
	char	*buf;

	buf = ft_strjoin(prog, ": ");
	info->win.header = ft_strjoin(buf, file);
	free(buf);
	info->win.res.x = RESX;
	info->win.res.y = RESY;
	info->win.win = mlx_new_window(info->mlx_ptr, info->win.res.x, info->win.res.y, info->win.header);
	info->win.cntr.x = info->win.res.x / 2;
	info->win.cntr.y = info->win.res.y / 2;
	info->data.res.x = RESX;
	info->data.res.y = RESY;
	info->data.img = mlx_new_image(info->mlx_ptr, info->data.res.x, info->data.res.y);
	info->data.cntr.x = info->data.res.x / 2;
	info->data.cntr.y = info->data.res.y / 2;
	// info->data.addr = mlx_get_data_addr(info->data.img,
	// 		&info->data.bits_per_pixel, &info->data.line_length,
	// 		&info->data.endian);
}

int	repairmouse(int x, int y, t_info *info)
{
	(void)x;
	mlx_mouse_move(info->win.win, 0, 0);
	if (!(y - 1 <= info->mouse.yshift && info->mouse.yshift <= y + 1))
	{
		info->mouse.yshift = y;
		return (0);
	}
	mlx_hook(info->win.win, 6, 1L << 6, &mousemove, info);
	return (0);
}

t_button	definebtn(int leftx, int lefty, int xsize, int ysize)
{
	t_button btn;

	btn.leftup.x = leftx;
	btn.leftup.y = lefty;
	btn.bottomright.x = leftx + xsize;
	btn.bottomright.y = lefty + ysize;
	return (btn);
}

void	definearrowframes(t_button arrows[20], int leftx, int rghtx, t_res *frame)
{
	arrows[0] = definebtn(frame->x + leftx, 38, 10, 20);
	arrows[1] = definebtn(frame->x + rghtx, 38, 10, 20);
	arrows[2] = definebtn(frame->x + leftx, 63, 10, 20);
	arrows[3] = definebtn(frame->x + rghtx, 63, 10, 20);
	arrows[4] = definebtn(frame->x + leftx, 203, 10, 20);
	arrows[5] = definebtn(frame->x + rghtx, 203, 10, 20);
	arrows[6] = definebtn(frame->x + leftx, 233, 10, 20);
	arrows[7] = definebtn(frame->x + rghtx, 233, 10, 20);
	arrows[8] = definebtn(frame->x + leftx, 253, 10, 20);
	arrows[9] = definebtn(frame->x + rghtx, 253, 10, 20);
	arrows[10] = definebtn(frame->x + leftx, 283, 10, 20);
	arrows[11] = definebtn(frame->x + rghtx, 283, 10, 20);
	arrows[12] = definebtn(frame->x + leftx, 393, 10, 20);
	arrows[13] = definebtn(frame->x + rghtx, 393, 10, 20);
	arrows[14] = definebtn(frame->x + leftx, 423, 10, 20);
	arrows[15] = definebtn(frame->x + rghtx, 423, 10, 20);
	arrows[16] = definebtn(frame->x + leftx, 453, 10, 20);
	arrows[17] = definebtn(frame->x + rghtx, 453, 10, 20);
	arrows[18] = definebtn(frame->x + leftx, 483, 10, 20);
	arrows[19] = definebtn(frame->x + rghtx, 483, 10, 20);
}

void	initinterface(t_intrfc *interface, void *mlx, t_res *win)
{
	interface->cam.img = mlx_xpm_file_to_image(mlx, "caminterface.xpm",
		&interface->cam.res.x, &interface->cam.res.y);
	interface->campos.x = win->x - interface->cam.res.x;
	interface->campos.y = 0;
	interface->obj.img = mlx_xpm_file_to_image(mlx, "objinterface.xpm",
		&interface->obj.res.x, &interface->obj.res.y);
	interface->objpos.x = win->x - interface->obj.res.x;
	interface->objpos.y = interface->cam.res.y;
	interface->frame.x = interface->campos.x;
	interface->frame.y = interface->objpos.y + interface->obj.res.y;
	interface->fov.step = 5 * M_PI / 360;
	interface->fov.stepping = 0.5 * M_PI / 180;
	interface->size.step = 0.1;
	interface->size.stepping = 0.1;
	interface->color.step = 10.f / 255.f;
	interface->color.stepping = 1.f / 255.f;
	interface->selected = NULL;
	definearrowframes(interface->arrows, 63, 93, &interface->frame);
}

int	main(int argc, char **argv)
{
	t_info	info;
	int		fd;

	info.prog = ft_strrchr(*argv++, '/') + 1;
	if (argc != 2)
		customerr(info.prog, INVINP, FALSE);
	fd = file_check(*argv, info.prog);
	info.a_light.determined = FALSE;
	info.lights.determined = FALSE;
	info.win.camera.determined = FALSE;
	info.mlx_ptr = mlx_init();
	wininit(&info, info.prog, *argv);
	ft_read_information(fd, &info);
	createframerouts(info.objects);
	info.win.camera.objs = NULL;
	createcamobjs(&info.win.camera.objs, info.objects);
	initview(info.objects, &info.win.camera, &info.lights);
	info.keybrd.render = FALSE;
	ft_draw_screen(&info);
	info.mouse.yshift = 0;
	mlx_hook(info.win.win, 2, 1L, &keydownhndlr, &info);
	mlx_hook(info.win.win, 3, 1L << 1, &keyuphndlr, &info);
	mlx_hook(info.win.win, 6, 1L << 6, &repairmouse, &info);
	mlx_loop(info.mlx_ptr);
	return (0);
}

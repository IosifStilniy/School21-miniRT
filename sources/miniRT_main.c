/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/09 22:58:13 by ncarob           ###   ########.fr       */
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
	info->data.img = mlx_new_image(info->mlx_ptr, RESX, RESY);
	info->data.addr = mlx_get_data_addr(info->data.img,
			&info->data.bits_per_pixel, &info->data.line_length,
			&info->data.endian);
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
	info.win.camera.camobjs.objs = NULL;
	info.win.camera.camobjs.inframe = NULL;
	info.win.camera.camobjs.outframe = NULL;
	createcamobjs(&info.win.camera.camobjs.objs, &info.win.camera.camobjs.outframe, info.objects);
	initview(info.objects, &info.win.camera);
	t_obj *obj;
	obj = info.win.camera.camobjs.objs->content;
	printf("crdstm: %.3f %.3f %.3f\n", obj->crdstm.pos.x, obj->crdstm.pos.y, obj->crdstm.pos.z);
	printf("dot1: %.3f %.3f %.3f\n", obj->dots.pos->dot.x, obj->dots.pos->dot.y, obj->dots.pos->dot.z);
	printf("rad: %.3f\n", obj->outframe);
	ft_draw_screen(&info);
	mlx_hook(info.win.win, 2, 1L, &keydownhndlr, &info);
	mlx_hook(info.win.win, 3, 1L << 1, &keyuphndlr, &info);
	mlx_hook(info.win.win, 6, 1L << 6, &mousemove, &info);
	mlx_loop(info.mlx_ptr);
	return (0);
}

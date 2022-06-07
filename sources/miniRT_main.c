/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/07 21:25:11 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	wininit(t_win *win, void *mlx, char *prog, char *file)
{
	char	*buf;

	buf = ft_strjoin(prog, ": ");
	win->header = ft_strjoin(buf, file);
	free(buf);
	win->res.x = RESX;
	win->res.y = RESY;
	win->win = mlx_new_window(mlx, win->res.x, win->res.y, win->header);
	win->cntr.x = win->res.x / 2;
	win->cntr.y = win->res.y / 2;
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
	wininit(&info.win, info.mlx_ptr, info.prog, *argv);
	ft_read_information(fd, &info);
	createcamobjs(&info.win.camera.camobjs.objs, &info.win.camera.camobjs.outframe, info.objects);
	initview(info.objects, &info.win.camera);
	// backwards_ray_tracing(&info);
	mlx_hook(info.win.win, 2, 1L, &keydownhndlr, &info);
	mlx_hook(info.win.win, 3, 1L << 1, &keyuphndlr, &info);
	mlx_hook(info.win.win, 6, 1L << 6, &mousemove, &info);
	mlx_loop(info.mlx_ptr);
	return (0);
}

#include "minirt.h"

void	paintgradline(t_cart dots[2], t_axis *color, int numstep, t_data *img)
{
	char	*pxl;
	int		x;
	int		y;

	while (numstep--)
	{
		x = lrintf(dots[0].x) + img->cntr.x;
		y = lrintf(dots[0].y) + img->cntr.y;
		pxl = img->addr + y * img->line_length + x * (img->bits_per_pixel / 8);
		*(t_ui *)pxl = ft_create_trgb(0, color->vector.x * 255, color->vector.y * 255, color->vector.z * 255);
		if (color->vector.z - color->ang > 0)
			color->vector.z -= color->ang;
		else if (color->vector.x - color->ang > 0)
			color->vector.x -= color->ang;
		else if (color->vector.y - color->ang > 0)
			color->vector.y -= color->ang;
		dots[0].x += dots[1].x;
		dots[0].y += dots[1].y;
		dots[0].z += dots[1].z;
	}
}

void	paintgradcircle(t_data *img, t_vrtx dots[RNDSGMNTS])
{
	t_cart	startdir[2];
	t_axis	color;
	int		numstep;
	int		i;

	startdir[1] = dots[1].dot;
	objtoobjpos(&dots[0].dot, &startdir[1]);
	numstep = lrintf(vectorlength(&startdir[1]));
	vectorbuilder(1, 1, 1, &color);
	vectorsizing(color.length, &color.vector, &color.vector, &color.length);
	color.ang = 3.f / RNDSGMNTS / numstep * 0.85f;
	i = -1;
	while (++i < RNDSGMNTS)
	{
		startdir[0] = dots[RNDSGMNTS - 1].dot;
		if (i)
			startdir[0] = dots[i - 1].dot;
		startdir[1] = dots[i].dot;
		objtoobjpos(&startdir[0], &startdir[1]);
		vectorsizing(1, &startdir[1], &startdir[1], NULL);
		paintgradline(startdir, &color, numstep, img);
	}
}

void	transparentimg(t_data *img)
{
	char	*pxl;
	int		x;
	int		y;

	x = -1;
	while (++x < img->res.x)
	{
		y = -1;
		while (++y < img->res.y)
		{
			pxl = img->addr + y * img->line_length + x * (img->bits_per_pixel / 8);
			*(t_ui *)pxl = 0xFF000000;
		}
	}
}

void	roundselected(t_cart *pos, float outframe, t_win *win, void *mlx)
{
	t_vrtx	dots[RNDSGMNTS];
	t_data	img;
	t_res	imgpos;
	int		size;

	outframe *= win->camera.focus / pos->z * 1.3;
	size = lrintf(ceilf(outframe)) * 2 + 10;
	if (size > win->res.x)
		return ;
	img.res.x = size;
	img.res.y = size;
	img.cntr.x = size / 2;
	img.cntr.y = size / 2;
	img.img = mlx_new_image(mlx, img.res.x, img.res.y);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	transparentimg(&img);
	circledotsfiller(dots, outframe, NULL, FALSE);
	paintgradcircle(&img, dots);
	imgpos.x = lrintf(pos->x * win->camera.focus / pos->z) + win->cntr.x - img.cntr.x;
	imgpos.y = lrintf(pos->y * win->camera.focus / pos->z) + win->cntr.y - img.cntr.y;
	mlx_put_image_to_window(mlx, win->win, img.img, imgpos.x, imgpos.y);
	mlx_destroy_image(mlx, img.img);
}

t_obj	*selectobject(t_list *camobjs, t_cart *vec)
{
	t_obj	*obj;
	t_cart	res;
	t_obj	*nearest;

	nearest = NULL;
	while (camobjs)
	{
		obj = camobjs->content;
		if (obj->crdstm.pos.z < 1 || !obj->dots.dotsnum)
		{
			camobjs = camobjs->next;
			continue ;
		}
		vectorsizing(obj->crdstm.pos.z / vec->z, vec, &res, NULL);
		objtoobjpos(&obj->crdstm.pos, &res);
		if (vectorlength(&res) > obj->outframe)
		{
			camobjs = camobjs->next;
			continue ;
		}
		if (!nearest || obj->crdstm.pos.z < nearest->crdstm.pos.z)
			nearest = obj;
		camobjs = camobjs->next;
	}
	return (nearest);
}

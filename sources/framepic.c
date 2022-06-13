#include "minirt.h"

t_bool	paintdot(t_cart *dot, t_cart *dir)
{
	long int	x;
	long int	y;

	if (dot->x < 0 && dir->x < 0)
		return (FALSE);
	if (dot->y < 0 && dir->y < 0)
		return (FALSE);
	if (dot->z < 0 && dir->z < 0)
		return (FALSE);
}

void	paintline(t_cart start, t_cart end, t_win *win, t_data *img)
{
	t_cart		dir;
	t_cart		xydir;
	float		length;
	float		step;
	long int	stepcount;

	if (start.z < 1 || end.z < 1)
		return ;
	start.x = start.x * win->camera.focus / start.z + win->cntr.x;
	start.y = start.y * win->camera.focus / start.z + win->cntr.y;
	end.x = end.x * win->camera.focus / end.z + win->cntr.x;
	end.y = end.y * win->camera.focus / end.z + win->cntr.y;
	dir = end;
	objtoobjpos(&start, &dir);
	length = vectorlength(&dir);
	cartbuilder(dir.x, dir.y, 0, &xydir);
	vectorsizing(1, &xydir, &xydir, NULL);
	cartbuilder(xydir.x, xydir.y, dir.z, &dir);
	step = vectorlength(&dir);
	stepcount = lrintf(length / step) + 1;
	while (stepcount-- && paintdot(&start, &dir))
	{
		start.x += dir.x;
		start.y += dir.y;
		start.z += dir.z;
	}
}

t_bool	objinframe(t_obj *obj, t_res *winctr, float focus)
{
	t_cart	objpos;
	t_cart	edgedir;
	float	dstnc;

	dstnc = obj->crdstm.pos.z;
	dstnc += comparef(dstnc, 0, 0.001);
	if (dstnc < 0 && fabsf(dstnc) > obj->outframe)
		return (FALSE);
	objpos = obj->crdstm.pos;
	cartbuilder(-objpos.x, -objpos.y, 0, &edgedir);
	if (vectorlength(&edgedir) < obj->outframe)
		return (TRUE);
	dottranslation(&objpos, &edgedir, obj->outframe);
	objpos.x *= objpos.x * focus / dstnc + winctr->x;
	objpos.y *= objpos.y * focus / dstnc + winctr->y;
	if (!(0 <= objpos.x && objpos.x <= RESX))
		return (FALSE);
	return ((0 <= objpos.y && objpos.y <= RESY));
}

void	framepic(t_win *win, t_list *camobjs, t_data *img, void *mlx)
{
	t_obj	*obj;
	t_bool	inframe;
	t_vrtx	*pos;
	int		i;

	img->img = mlx_new_image(mlx, RESX, RESY);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	ft_bzero(img->addr, img->line_length * RESY);
	while (camobjs)
	{
		obj = camobjs->content;
		inframe = obj->dots.dotsnum;
		if (inframe)
			inframe = objinframe(obj, &win->cntr, win->camera.focus);
		pos = obj->dots.pos;
		i = -1;
		while (inframe && ++i < obj->dots.routsize)
			paintline(pos[obj->dots.rout[i][0]].dot,
				pos[obj->dots.rout[i][1]].dot, win, img);
		camobjs = camobjs->next;
	}
	mlx_put_image_to_window(mlx, win->win, img->img, 0, 0);
	mlx_destroy_image(mlx, img->img);
}

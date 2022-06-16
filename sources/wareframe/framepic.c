#include "minirt.h"

void	paintdot(t_cart *dot, t_cart *dir, t_data *img, t_bool *painted)
{
	long int	x;
	long int	y;
	char		*pxl;

	*painted = ((dot->x < 0 && dir->x < 0) || (dot->x > RESX - 1 && dir->x > 0));
	*painted += ((dot->y < 0 && dir->y < 0) || (dot->y > RESY - 1 && dir->y > 0));
	if ((*painted))
		return ;
	if (dot->x < 0 || dot->x > RESX - 1)
		return ;
	if (dot->y < 0 || dot->y > RESY - 1)
		return ;
	x = lrintf(dot->x);
	y = lrintf(dot->y);
	pxl = img->addr + y * img->line_length + x * (img->bits_per_pixel / 8);
	*(t_ui *)pxl = FRAMECLR;
}

long int	dirdefiner(t_cart *start, t_cart *dir)
{
	t_cart		xydir;
	float		length;

	objtoobjpos(start, dir);
	length = vectorlength(dir);
	vectorsizing(1, dir, dir, NULL);
	cartbuilder(dir->x, dir->y, 0, &xydir);
	vectorsizing(1 / vectorlength(&xydir), dir, dir, NULL);
	return (ceilf(length / vectorlength(dir)) + 1);
}

void	paintline(t_cart start, t_cart end, float focus, t_data *img)
{
	t_cart		dir;
	t_bool		painted;
	long int	stepcount;

	if (start.z < 1 || end.z < 1)
		return ;
	start.x = start.x * focus / start.z + img->cntr.x;
	start.y = start.y * focus / start.z + img->cntr.y;
	end.x = end.x * focus / end.z + img->cntr.x;
	end.y = end.y * focus / end.z + img->cntr.y;
	if ((start.x < 0 && end.x < 0) || (start.x > RESX - 1 && end.x > RESX - 1))
		return ;
	if ((start.y < 0 && end.y < 0) || (start.y > RESY - 1 && end.y > RESY - 1))
		return ;
	dir = end;
	stepcount = dirdefiner(&start, &dir);
	painted = FALSE;
	while (stepcount-- && !painted)
	{
		paintdot(&start, &dir, img, &painted);
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
	objpos.x = objpos.x * focus / dstnc + winctr->x;
	objpos.y = objpos.y * focus / dstnc + winctr->y;
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

	img->img = mlx_new_image(mlx, img->res.x, img->res.y);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
	ft_bzero(img->addr, img->line_length * RESY);
	while (camobjs)
	{
		obj = camobjs->content;
		if (!obj->dots.dotsnum)
			planeframing(obj, &win->camera, img);
		inframe = objinframe(obj, &win->cntr, win->camera.focus);
		pos = obj->dots.pos;
		i = -1;
		while (obj->dots.dotsnum && inframe && ++i < obj->dots.routsize)
			paintline(pos[obj->dots.rout[i][0]].dot,
				pos[obj->dots.rout[i][1]].dot, win->camera.focus, img);
		camobjs = camobjs->next;
	}
	mlx_put_image_to_window(mlx, win->win, img->img, win->cntr.x - img->cntr.x, win->cntr.y - img->cntr.y);
	mlx_destroy_image(mlx, img->img);
}

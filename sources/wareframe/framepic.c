/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framepic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:10:21 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 21:41:09 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	paintdot(t_cart dotdir[2], t_ui color, t_data *img, t_bool *painted)
{
	long int	x;
	long int	y;
	char		*pxl;

	*painted = ((dotdir[0].x < 0 && dotdir[1].x < 0)
			|| (dotdir[0].x > RESX - 1 && dotdir[1].x > 0));
	*painted += ((dotdir[0].y < 0 && dotdir[1].y < 0)
			|| (dotdir[0].y > RESY - 1 && dotdir[1].y > 0));
	if (*painted)
		return ;
	if (dotdir[0].x < 0 || dotdir[0].x > RESX - 1)
		return ;
	if (dotdir[0].y < 0 || dotdir[0].y > RESY - 1)
		return ;
	x = lrintf(dotdir[0].x);
	y = lrintf(dotdir[0].y);
	pxl = img->addr + y * img->line_length + x * (img->bits_per_pixel / 8);
	*(t_ui *)pxl = color;
}

long int	dirdefiner(t_cart *start, t_cart *dir)
{
	t_cart		xydir;
	float		length;

	objtoobjpos(start, dir);
	length = vectorlength(dir);
	if (comparef(length, 0, 0.001))
		return (1);
	vectorsizing(1, dir, dir, NULL);
	cartbuilder(dir->x, dir->y, 0, &xydir);
	if (comparef(vectorlength(&xydir), 0, 0.001))
		return (1);
	vectorsizing(1 / vectorlength(&xydir), dir, dir, NULL);
	return (ceilf(length / vectorlength(dir)) + 1);
}

void	paintline(t_cart src[2], t_ui color, float focus, t_data *img)
{
	t_bool		painted;
	long int	stepcount;

	if (src[0].z < 1 || src[1].z < 1)
		return ;
	src[0].x = src[0].x * focus / src[0].z + img->cntr.x;
	src[0].y = src[0].y * focus / src[0].z + img->cntr.y;
	src[1].x = src[1].x * focus / src[1].z + img->cntr.x;
	src[1].y = src[1].y * focus / src[1].z + img->cntr.y;
	if ((src[0].x < 0 && src[1].x < 0)
		|| (src[0].x > RESX - 1 && src[1].x > RESX - 1)
		|| (src[0].y < 0 && src[1].y < 0)
		|| (src[0].y > RESY - 1 && src[1].y > RESY - 1))
		return ;
	stepcount = dirdefiner(&src[0], &src[1]);
	painted = FALSE;
	while (stepcount-- && !painted)
	{
		paintdot(src, color, img, &painted);
		src[0].x += src[1].x;
		src[0].y += src[1].y;
		src[0].z += src[1].z;
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
	return (0 <= objpos.y && objpos.y <= RESY);
}

void	framepic(t_win *win, t_bool normalprint, t_list *camobjs, t_data *img)
{
	t_obj	*obj;
	t_bool	inframe;
	t_ui	color;

	while (camobjs)
	{
		obj = camobjs->content;
		if (!obj->dots.dotsnum)
			planeframing(obj, win->camera, img);
		inframe = objinframe(obj, &win->cntr, win->camera->focus);
		color = ft_create_trgb(0, obj->colrs->x * 255, obj->colrs->y * 255,
				obj->colrs->z * 255);
		if (inframe && obj->dots.dotsnum)
			framepainter(&obj->dots, win->camera->focus, img, color);
		if (inframe && normalprint && obj->dots.dotsnum)
			normpainter(obj, win->camera->focus, img);
		camobjs = camobjs->next;
	}
}

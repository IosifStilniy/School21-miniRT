#include "minirt.h"

void	lineprinter(t_cart startenddir[3], t_ui color, float focus, t_data *img)
{
	t_cart	grid[2];
	t_cart	paint[2];
	int		i;

	i = -1;
	grid[0] = startenddir[0];
	grid[1] = startenddir[1];
	while (++i < GRIDLINES - 1)
	{
		dottranslation(&grid[0], &startenddir[2], GRIDSIZE);
		dottranslation(&grid[1], &startenddir[2], GRIDSIZE);
		paint[0] = grid[0];
		paint[1] = grid[1];
		paintline(paint, color, focus, img);
	}
}

void	gridprinter(t_cart *start, t_obj *plane, float focus, t_data *img)
{
	t_cart	dst[3];
	t_cart	end;
	t_ui	color;

	color = ft_create_trgb(0, plane->colrs->x * 255, plane->colrs->y * 255, plane->colrs->z * 255);
	dotcrdstmtrnsltn(start, &dst[0], NULL, &plane->crdstm);
	vectodot(&dst[0], &plane->crdstm.pos);
	cartbuilder(start->x + GRIDLINES * GRIDSIZE, start->y, 0, &end);
	dotcrdstmtrnsltn(&end, &dst[1], NULL, &plane->crdstm);
	vectodot(&dst[1], &plane->crdstm.pos);
	dst[2] = plane->crdstm.oy.vector;
	lineprinter(dst, color, focus, img);
	cartbuilder(start->x, start->y + GRIDLINES * GRIDSIZE, 0, &end);
	dotcrdstmtrnsltn(&end, &dst[1], NULL, &plane->crdstm);
	vectodot(&dst[1], &plane->crdstm.pos);
	dst[2] = plane->crdstm.ox.vector;
	lineprinter(dst, color, focus, img);
}

void	gridbuilder(t_obj *plane, t_vrtx *pos, float focus, t_data *img)
{
	t_cart	start;
	int		i;

	i = -1;
	while (++i < CRNRS)
	{
		if (pos[i].dot.x == INFINITY || pos[i].dot.y == INFINITY)
			continue ;
		start.x = (lrintf(pos[i].dot.x) / GRIDSIZE - GRIDLINES / 2) * GRIDSIZE;
		start.y = (lrintf(pos[i].dot.y) / GRIDSIZE - GRIDLINES / 2) * GRIDSIZE;
		start.z = 0;
		gridprinter(&start, plane, focus,img);
	}
}

void	planeframing(t_obj *plane, t_camera *camera, t_data *img)
{
	t_crdstm	cam;
	t_trans		trans;
	t_cart		corners[CRNRS];
	t_bool		inframe;
	int			i;

	cartbuilder(0, 0, 0, &cam.pos);
	cartbuilder(0, 0, 1, &cam.oz.vector);
	crdstmdefiner(&cam);
	worldtocammatrix(trans.trans, trans.crdstm, trans.pos, &plane->crdstm);
	transpos(&cam.pos, trans.trans);
	transpos(&cam.ox.vector, trans.crdstm);
	transpos(&cam.oy.vector, trans.crdstm);
	transpos(&cam.oz.vector, trans.crdstm);
	inframe = FALSE;
	i = -1;
	while (++i < CRNRS)
	{
		cartbuilder(INFINITY, INFINITY, INFINITY, &plane->dots.pos[i].dot);
		dotcrdstmtrnsltn(&camera->corners[i], &corners[i], NULL, &cam);
		if (corners[i].z * cam.pos.z > 0 || !++inframe)
			continue ;
		vectorsizing(fabsf(cam.pos.z / corners[i].z), &corners[i], &plane->dots.pos[i].dot, NULL);
		vectodot(&plane->dots.pos[i].dot, &cam.pos);
	}
	if (inframe)
		gridbuilder(plane, plane->dots.pos, camera->focus, img);
}

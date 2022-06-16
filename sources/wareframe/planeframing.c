#include "minirt.h"

void	lineprinter(t_cart startend[2], t_cart *dir, float focus, t_data *img)
{
	t_cart	start;
	t_cart	end;
	int		i;

	start = startend[0];
	end = startend[1];
	i = -1;
	while (++i < GRIDLINES - 1)
	{
		dottranslation(&start, dir, GRIDSIZE);
		dottranslation(&end, dir, GRIDSIZE);
		paintline(start, end, focus, img);
	}
}

void	gridbuilder(t_crdstm *plane, t_vrtx *pos, float focus, t_data *img)
{
	t_cart	start;
	t_cart	end;
	t_cart	dst[2];
	int		i;

	i = -1;
	while (++i < CRNRS)
	{
		if (pos[i].dot.x == INFINITY || pos[i].dot.y == INFINITY)
			continue ;
		cartbuilder((lrintf(pos[i].dot.x) / GRIDSIZE - GRIDLINES / 2) * GRIDSIZE,
					(lrintf(pos[i].dot.y) / GRIDSIZE - GRIDLINES / 2) * GRIDSIZE,
					0, &start);
		dotcrdstmtrnsltn(&start, &dst[0], 1, plane);
		vectodot(&dst[0], &plane->pos, FALSE);
		cartbuilder(start.x + GRIDLINES * GRIDSIZE, start.y, 0, &end);
		dotcrdstmtrnsltn(&end, &dst[1], 1, plane);
		vectodot(&dst[1], &plane->pos, FALSE);
		lineprinter(dst, &plane->oy.vector, focus, img);
		cartbuilder(start.x, start.y + GRIDLINES * GRIDSIZE, 0, &end);
		dotcrdstmtrnsltn(&end, &dst[1], 1, plane);
		vectodot(&dst[1], &plane->pos, FALSE);
		lineprinter(dst, &plane->ox.vector, focus, img);
	}
}

void	planeframing(t_obj *plane, t_camera *camera, t_data *img)
{
	t_crdstm	cam;
	t_cart		corners[CRNRS];
	t_bool		inframe;
	int			i;

	cartbuilder(0, 0, 1, &cam.oz.vector);
	crdstmdefiner(&cam);
	cartbuilder(-plane->crdstm.pos.x, -plane->crdstm.pos.y, -plane->crdstm.pos.z, &cam.pos);
	objtoobjaxis(WORLD, &plane->crdstm, plane->rot);
	crdstmrotbyaxis(&cam, &plane->rot->axis, &plane->rot->xyaxis);
	quartrot(&cam.pos, &plane->rot->axis);
	quartrot(&cam.pos, &plane->rot->xyaxis);
	inframe = FALSE;
	i = -1;
	while (++i < CRNRS)
	{
		cartbuilder(INFINITY, INFINITY, INFINITY, &plane->dots.pos[i].dot);
		dotcrdstmtrnsltn(&camera->corners[i], &corners[i], 1, &cam);
		if (corners[i].z * cam.pos.z > 0 || !++inframe)
			continue ;
		vectorsizing(fabsf(cam.pos.z / corners[i].z), &corners[i], &plane->dots.pos[i].dot, NULL);
		vectodot(&plane->dots.pos[i].dot, &cam.pos, FALSE);
	}
	if (inframe)
		gridbuilder(&plane->crdstm, plane->dots.pos, camera->focus, img);
}

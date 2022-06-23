#include "minirt.h"

void	camshifting(t_camera *camera, t_info *info, t_cart *objsdir, float step)
{
	t_cart	camdir;

	camdir = camera->crdstm.ox.vector;
	if (objsdir->y)
		camdir = camera->crdstm.oy.vector;
	else if (objsdir->z)
		camdir = camera->crdstm.oz.vector;
	if (objsdir->x + objsdir->y + objsdir->z > 0)
		negativevector(&camdir);
	vectorsizing(step, &camdir, &camdir, NULL);
	vectodot(&camera->crdstm.pos, &camdir, FALSE);
	initview(info->objects, camera, &info->lights);
}

void	camrotating(t_camera *camera, t_info *info, int x, int y)
{
	t_axis	curpos;
	t_cart	oz;
	t_axis	axis;

	cartbuilder(x, -y, 1000, &curpos.vector);
	vectorsizing(1, &curpos.vector, &curpos.vector, NULL);
	cartbuilder(0, 0, 1, &oz);
	axisbuilder(&oz, &curpos.vector, &axis);
	if (comparef(axis.ang, 0, 0.1 * M_PI / 180))
		return ;
	dotcrdstmtrnsltn(&axis.vector, &curpos.vector, 1, &camera->crdstm);
	curpos.ang = axis.ang;
	crdstmrotbyaxis(&camera->crdstm, &curpos, NULL);
	initview(info->objects, camera, &info->lights);
}

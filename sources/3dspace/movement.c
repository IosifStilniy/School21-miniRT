#include "minirt.h"

void	camfromobjcrdstm(t_crdstm *cam, t_attach *attached)
{
	dotcrdstmtrnsltn(&attached->crdstm.pos, &cam->pos, NULL, &attached->obj->crdstm);
	dotcrdstmtrnsltn(&attached->crdstm.ox.vector, &cam->ox.vector, NULL, &attached->obj->crdstm);
	dotcrdstmtrnsltn(&attached->crdstm.oy.vector, &cam->oy.vector, NULL, &attached->obj->crdstm);
	dotcrdstmtrnsltn(&attached->crdstm.oz.vector, &cam->oz.vector, NULL, &attached->obj->crdstm);
}

void	camshifting(t_camera *camera, t_info *info, t_cart *dir, float step)
{
	t_cart	camdir;

	camdir = camera->crdstm.ox.vector;
	if (dir->y)
		camdir = camera->crdstm.oy.vector;
	else if (dir->z)
		camdir = camera->crdstm.oz.vector;
	if (dir->x + dir->y + dir->z < 0)
		negativevector(&camdir);
	vectorsizing(step, &camdir, &camdir, NULL);
	if (camera->attached.obj)
	{
		vectodot(&camera->attached.obj->crdstm.pos, &camdir, FALSE);
		camfromobjcrdstm(&camera->crdstm, &camera->attached);
	}
	else
		vectodot(&camera->crdstm.pos, &camdir, FALSE);
	initview(info->objects, camera, &info->lights);
}

void	camrotating(t_camera *camera, t_info *info, int x, int y)
{
	t_axis	curpos;
	t_cart	oz;
	t_axis	axis;

	cartbuilder(x, y, 1000 * info->interface.settings.sens.mval, &curpos.vector);
	vectorsizing(1, &curpos.vector, &curpos.vector, NULL);
	cartbuilder(0, 0, 1, &oz);
	axisbuilder(&oz, &curpos.vector, &axis);
	if (comparef(axis.ang, 0, 0.1 * M_PI / 180))
		return ;
	dotcrdstmtrnsltn(&axis.vector, &curpos.vector, NULL, &camera->crdstm);
	curpos.ang = axis.ang;
	if (camera->attached.obj)
	{
		crdstmrotbyaxis(&camera->attached.obj->crdstm, &curpos, NULL);
		camfromobjcrdstm(&camera->crdstm, &camera->attached);
	}
	else
		crdstmrotbyaxis(&camera->crdstm, &curpos, NULL);
	initview(info->objects, camera, &info->lights);
}

#include "minirt.h"

void	camshifting(t_camera *camera, t_list *objs, t_cart *objsdir, float step)
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
	initview(objs, camera);
}

void	camrotating(t_camera *camera, t_list *objs, int x, int y)
{
	t_axis	curpos;
	t_cart	oz;
	t_axis	axis;

	cartbuilder(x, y, 1000, &curpos.vector);
	vectorsizing(1, &curpos.vector, &curpos.vector, NULL);
	cartbuilder(0, 0, 1, &oz);

	// t_axis	asd;
	// axisbuilder(&camera->crdstm.oz.vector, &oz, &asd);
	// printf("cam oz -> world oz: %.3f %.3f %.3f %.3f\n", asd.vector.x, asd.vector.y, asd.vector.z, asd.ang * 180 / M_PI);
	
	// printf("cam crdstm:\n");
	// axisbuilder(&camera->crdstm.oz.vector, &camera->crdstm.ox.vector, &asd);
	// printf("oz->ox: %.3f %.3f %.3f %.3f\n", asd.vector.x, asd.vector.y, asd.vector.z, asd.ang * 180 / M_PI);
	// axisbuilder(&camera->crdstm.oz.vector, &camera->crdstm.oy.vector, &asd);
	// printf("oz->oy: %.3f %.3f %.3f %.3f\n", asd.vector.x, asd.vector.y, asd.vector.z, asd.ang * 180 / M_PI);
	// axisbuilder(&camera->crdstm.ox.vector, &camera->crdstm.oy.vector, &asd);
	// printf("ox->oy: %.3f %.3f %.3f %.3f\n", asd.vector.x, asd.vector.y, asd.vector.z, asd.ang * 180 / M_PI);
	axisbuilder(&oz, &curpos.vector, &axis);
	if (comparef(axis.ang, 0, 0.1 * M_PI / 180))
		return ;
	dotcrdstmtrnsltn(&axis.vector, &curpos.vector, 1, &camera->crdstm);
	curpos.ang = axis.ang;
	printf("cam crdstm:\n");
	printf("ox: %.3f %.3f %.3f\n", camera->crdstm.ox.vector.x, camera->crdstm.ox.vector.y, camera->crdstm.ox.vector.z);
	printf("oy: %.3f %.3f %.3f\n", camera->crdstm.oy.vector.x, camera->crdstm.oy.vector.y, camera->crdstm.oy.vector.z);
	printf("oz: %.3f %.3f %.3f\n", camera->crdstm.oz.vector.x, camera->crdstm.oz.vector.y, camera->crdstm.oz.vector.z);
	printf("axis: %.3f %.3f %.3f %.3f\n", axis.vector.x, axis.vector.y, axis.vector.z, axis.ang * 180 / M_PI);
	printf("curpos: %.3f %.3f %.3f\n", curpos.vector.x, curpos.vector.y, curpos.vector.z);
	crdstmrotbyaxis(&camera->crdstm, &curpos, NULL);
	initview(objs, camera);
}

#include "minirt.h"

void	objrot(t_obj *camobj, t_crdstm *cam, t_crdstm *obj, t_cart *dst)
{
	t_axis	refaxis;
	int		i;

	vectorbuilder(0, 0, 1, camobj->rot->start);
	camobj->rot->end = dst;
	if (!dst)
		crdstmrotbyaxis(&camobj->crdstm, &camobj->rot->axis);
	else
		crdstmrot(&camobj->crdstm, camobj->rot, camobj->rot->start, dst);
	refaxis = camobj->rot->axis;
	engine(&camobj->dots, &camobj->polys, &camobj->crdstm, camobj->rot);
	objtoobjaxis(cam, NULL, camobj->rot);
	quartrot(&refaxis.vector, &camobj->rot->axis);
	crdstmrotbyaxis(obj, &refaxis);
}

void	crdstmtranslation(t_cart *crdstm, t_cart *direction, float step)
{
	t_axis	res;

	vectorsizing(step, direction, &res);
	crdstm->x += res.vector.x;
	crdstm->y += res.vector.y;
	crdstm->z += res.vector.z;
}

void	crdstmrot(t_crdstm *crdstm, t_rot *rot, t_cart *start, t_cart *end)
{
	float	ang;
	t_axis	ref;

	rot->start = start;
	rot->end = end;
	flatanglehandler(rot, NULL);
	if (&crdstm->ox.vector == start || &crdstm->oy.vector == start)
	{
		ang = rot->axis.ang;
		axisbuilder(&crdstm->oz.vector, &rot->axis.vector, &ref);
		rot->axis = crdstm->oz;
		if (comparef(ref.ang, M_PI, 0.001))
			negativevector(&rot->axis.vector);
		rot->axis.ang = ang;
	}
	crdstmrotbyaxis(crdstm, &rot->axis);
}

void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *axis)
{
	quartrot(&crdstm->ox.vector, axis);
	quartrot(&crdstm->oy.vector, axis);
	quartrot(&crdstm->oz.vector, axis);
}

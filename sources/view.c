#include "minirt.h"

t_bool	collisiondefiner(t_cart *collisiondot, t_cart *vec, t_cart *pos)
{
	t_axis	dot;

	cartbuilder(INFINITY, INFINITY, INFINITY, collisiondot);
	axisbuilder(vec, pos, &dot);
	if (dot.ang < M_PI_2 - 0.001)
		return (FALSE);
	vectorsizing(fabsf(pos->z) / fabsf(vec->z), vec, &dot.vector, &dot.length);
	vectodot(&dot.vector, pos, FALSE);
	cartcopy(&dot.vector, collisiondot, 1);
	return (TRUE);
}

t_bool	planeinframe(t_obj *obj, t_cart camref, t_cart corners[4])
{
	t_cart		cornersref[4];
	t_bool		hascollision;
	t_rot		axis;
	int			i;

	objtoobjaxis(WORLD, &obj->crdstm, &axis);
	negativevector(&camref);
	quartrot(&camref, &axis.axis);
	quartrot(&camref, &axis.xyaxis);
	hascollision = FALSE;
	i = -1;
	while (++i < 4)
	{
		cornersref[i] = corners[i];
		quartrot(cornersref + i, &axis.axis);
		quartrot(cornersref + i, &axis.xyaxis);
		hascollision += collisiondefiner(obj->dots.pos + i, cornersref + i, &camref);
	}
	return (hascollision);
}

t_bool	objinframe(t_obj *obj, t_camera *camera, t_res *cntr)
{
	t_axis	v1;
	t_axis	v2;
	t_axis	edge;

	if (!obj->dots.dotsnum)
		return (planeinframe(obj, obj->crdstm.pos, camera->corners));
	vectorbuilder(obj->crdstm.pos.x, obj->crdstm.pos.y, obj->crdstm.pos.z, &v1);
	if (obj->crdstm.pos.z < 0.001)
		return (v1.length < obj->outframe);
	axisbuilder(&v1, &camera->crdstm.oz.vector, &v2);
	vectorsizing(v1.length * cosf(v2.ang), &camera->crdstm.oz.vector, &v2.vector, &v2.length);
	vectortoobj(&v1.vector, &v2.vector, &edge);
	if (obj->outframe > edge.length - 0.001)
		return (TRUE);
	vectorsizing(obj->outframe, &edge.vector, &edge.vector, &edge.length);
	vectodot(&edge.vector, &obj->crdstm.pos, FALSE);
	vectorbuilder(0, edge.vector.y, edge.vector.z, &v1);
	axisbuilder(&v1, &edge, &v2);
	if (v2.ang > camera->view.xfov - 0.001)
		return (FALSE);
	vectorbuilder(edge.vector.x, 0, edge.vector.z, &v1);
	axisbuilder(&v1, &edge, &v2);
	return (v2.ang > camera->view.yfov - 0.001);
}

void	rotatedview(t_obj *camobj, t_rot *rot, t_camera *camera, t_res *wincntr)
{
	objtoobjpos(&camera->crdstm.pos, &camobj->crdstm.pos);
	quartrot(&camobj->crdstm.pos, &rot->axis);
	quartrot(&camobj->crdstm.pos, &rot->xyaxis);
	crdstmrotbyaxis(&camobj->crdstm, &rot->axis, &rot->xyaxis);
	if (objinframe(camobj, camera, wincntr))
		objexchanger(camobj, &camera->camobjs.inframe, &camera->camobjs.outframe);
	else
		objexchanger(camobj, &camera->camobjs.outframe, &camera->camobjs.inframe);
	engine(&camobj->dots, &camobj->polys, &camobj->crdstm);
}

void	shiftedview(t_obj *camobj, t_camera *camera, t_res *wincntr)
{
	t_shift	*shift;

	shift = &camobj->move->shift;
	dotstranslation(&camobj->crdstm.pos, 1, shift->direction, shift->step);
	vrtxtranslation(camobj->dots.pos, camobj->dots.dotsnum, shift->direction, shift->step);
	if (objinframe(camobj, camera, wincntr))
		objexchanger(camobj, &camera->camobjs.inframe, &camera->camobjs.outframe);
	else
		objexchanger(camobj, &camera->camobjs.outframe, &camera->camobjs.inframe);
}

#include "minirt.h"

t_bool	collisiondefiner(t_cart *collisiondot, t_cart *vec, t_cart *pos)
{
	t_axis	dot;

	if (vec->z * pos->z >= 0)
		return (FALSE);
	vectorsizing(fabsf(pos->z) / fabsf(vec->z), vec, &dot);
	vectodot(&dot.vector, pos);
	cartcopy(&dot.vector, collisiondot, 1);
	return (TRUE);
}

t_bool	planeinframe(t_obj *obj, t_cart camref, t_cart corners[4])
{
	t_cart		cornersref[4];
	t_cart		z;
	t_bool		hascollision;
	t_rot		axis;
	int			i;

	objtoobjaxis(NULL, &obj->crdstm, &axis);
	crdstmrotbyaxis(&camref, &axis.axis, &axis.xyaxis);
	camref = obj->crdstm.pos;
	negativevector(&camref);
	quartrot(&camref, &axis.axis);
	quartrot(&camref, &axis.xyaxis);
	cartbuilder(0, 0, 1, &z);
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
		return (planeinframe(obj, camera->crdstm.pos, camera->corners));
	vectorbuilder(obj->crdstm.pos.x, obj->crdstm.pos.y, obj->crdstm.pos.z, &v1);
	if (obj->crdstm.pos.z < 0.001)
		return (v1.length < obj->outframe);
	axisbuilder(&v1, &camera->crdstm.oz.vector, &v2);
	vectorsizing(v1.length * cosf(v2.ang), &camera->crdstm.oz.vector, &v2);
	vectortoobj(&v1.vector, &v2.vector, &edge);
	if (obj->outframe > edge.length - 0.001)
		return (TRUE);
	vectorsizing(obj->outframe, &edge.vector, &edge);
	vectodot(&edge.vector, &obj->crdstm.pos);
	vectorbuilder(0, edge.vector.y, edge.vector.z, &v1);
	axisbuilder(&v1, &edge, &v2);
	if (v2.ang > camera->xfov - 0.001)
		return (FALSE);
	vectorbuilder(edge.vector.x, 0, edge.vector.z, &v1);
	axisbuilder(&v1, &edge, &v2);
	return (v2.ang > camera->yfov - 0.001);
}

void	createview(t_list *objs, t_camera *camera, t_res *wincntr)
{
	t_list	*crsr;
	t_list	*camcrsr;
	t_obj	*camobj;
	t_rot	rot;
	int		i;

	objtoobjaxis(NULL, &camera->crdstm, &rot);
	crsr = objs;
	camcrsr = camera->camobjs;
	i = -1;
	while (crsr)
	{
		camobj = objcast(camcrsr);
		crdstmcopy(&objcast(crsr)->crdstm, &camobj->crdstm);
		objtoobjpos(&camera->crdstm.pos, &camobj->crdstm.pos);
		quartrot(&camobj->crdstm.pos, &rot.axis);
		quartrot(&camobj->crdstm.pos, &rot.xyaxis);
		crdstmrotbyaxis(&camobj->crdstm, &rot.axis, &rot.xyaxis);
		camera->objsinframe[++i] = objinframe(camobj, camera, wincntr);
		if (camera->objsinframe[i] && camobj->dots.dotsnum)
			engine(&camobj->dots, &camobj->polys, &camobj->crdstm, &camobj->rot);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

void	createcamobjs(t_list **camobjs, t_list *objs, t_bool **objsinframe)
{
	t_obj	*camobj;
	t_obj	*obj;

	*objsinframe = malloc(sizeof(**objsinframe) * ft_lstsize(objs));
	ft_bzero(*objsinframe, ft_lstsize(objs));
	while (objs)
	{
		obj = objcast(objs);
		ft_lstadd_front(camobjs, ft_lstnew(malloc(sizeof(*camobj))));
		camobj = objcast(*camobjs);
		camobj->rot = obj->rot;
		camobj->dots.dotsnum = obj->dots.dotsnum;
		camobj->dots.dots = obj->dots.dots;
		if (camobj->dots.dotsnum)
			camobj->dots.pos = malloc(sizeof(*camobj->dots.pos) * camobj->dots.dotsnum);
		else
			camobj->dots.pos = malloc(sizeof(*camobj->dots.pos) * 4);
		camobj->dots.scale = obj->dots.scale;
		camobj->polys.poly = obj->polys.poly;
		camobj->polys.txtr = obj->polys.txtr;
		camobj->polys.polynum = obj->polys.polynum;
		objs = objs->next;
	}
}

#include "minirt.h"

void	camshifting(t_crdstm *camera, t_list *camobjs, t_cart *objsdir, float step)
{
	t_obj	*obj;
	t_cart	res;
	t_cart	camdir;

	cartcopy(&camera->ox.vector, &camdir, 1);
	if (objsdir->y)
		cartcopy(&camera->oy.vector, &camdir, 1);
	else if (objsdir->z)
		cartcopy(&camera->oz.vector, &camdir, 1);
	vectorsizing(step, &camdir, &res, NULL);
	vectodot(&camera->pos, &res, FALSE);
	vectorsizing(step, objsdir, &res, NULL);
	while (camobjs)
	{
		obj = objcast(camobjs);
		vectodot(&obj->crdstm.pos, &res, FALSE);
		if (obj->dots.dotsnum)
			vrtxtranslation(obj->dots.pos, obj->dots.dotsnum, objsdir, step);
		camobjs = camobjs->next;
	}
}

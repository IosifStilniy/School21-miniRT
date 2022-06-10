#include "minirt.h"

void	camshifting(t_camera *camera, t_cart *camdir, t_cart *objsdir, float step)
{
	t_obj	*obj;
	t_list	*crsr;
	t_cart	res;

	vectorsizing(step, camdir, &res, NULL);
	camera->crdstm.pos.x += res.x;
	camera->crdstm.pos.y += res.y;
	camera->crdstm.pos.z += res.z;
	vectorsizing(step, objsdir, &res, NULL);
	crsr = camera->objs;
	while (crsr)
	{
		obj = objcast(crsr);
		obj->crdstm.pos.x += res.x;
		obj->crdstm.pos.y += res.y;
		obj->crdstm.pos.z += res.z;
		vrtxtranslation(obj->dots.pos, obj->dots.dotsnum, objsdir, step);
		crsr = crsr->next;
	}
}

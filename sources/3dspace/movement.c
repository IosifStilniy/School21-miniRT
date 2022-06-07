#include "minirt.h"

void	camshifting(t_camera *camera, t_cart *camdir, t_cart *objsdir, float step)
{
	t_obj	*obj;
	t_list	*crsr;

	camera->crdstm.pos.x += camdir->x;
	camera->crdstm.pos.y += camdir->y;
	camera->crdstm.pos.z += camdir->z;
	crsr = camera->camobjs.objs;
	while (crsr)
	{
		obj = objcast(crsr);
		obj->crdstm.pos.x += objsdir->x;
		obj->crdstm.pos.y += objsdir->y;
		obj->crdstm.pos.z += objsdir->z;
		vrtxtranslation(obj->dots.pos, obj->dots.dotsnum, objsdir, step);
		crsr = crsr->next;
	}
}

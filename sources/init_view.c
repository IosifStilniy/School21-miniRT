#include "minirt.h"

void	createcamobjs(t_list **camobjs, t_list **outframe, t_list *objs)
{
	t_obj	*camobj;
	t_obj	*obj;

	while (objs)
	{
		obj = objcast(objs);
		ft_lstadd_front(camobjs, ft_lstnew(malloc(sizeof(*camobj))));
		ft_lstadd_front(outframe, ft_lstnew((*camobjs)->content));
		camobj = objcast(*camobjs);
		camobj->rot = obj->rot;
		camobj->outframe = obj->outframe;
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

void	initview(t_list *objs, t_camera *camera)
{
	t_list	*crsr;
	t_list	*camcrsr;
	t_obj	*camobj;
	t_rot	rot;

	objtoobjaxis(WORLD, &camera->crdstm, &rot);
	crsr = objs;
	camcrsr = camera->camobjs.objs;
	while (crsr)
	{
		camobj = objcast(camcrsr);
		crdstmcopy(&objcast(crsr)->crdstm, &camobj->crdstm);
		objtoobjpos(&camera->crdstm.pos, &camobj->crdstm.pos);
		quartrot(&camobj->crdstm.pos, &rot.axis);
		quartrot(&camobj->crdstm.pos, &rot.xyaxis);
		crdstmrotbyaxis(&camobj->crdstm, &rot.axis, &rot.xyaxis);
		if (objinframe(camobj, camera))
			objexchanger(camcrsr, &camera->camobjs.inframe, &camera->camobjs.outframe, &camera->camobjs);
		else
			objexchanger(camcrsr, &camera->camobjs.outframe, &camera->camobjs.inframe, &camera->camobjs);
		engine(&camobj->dots, &camobj->polys, &camobj->crdstm);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

#include "minirt.h"

void	createcamobjs(t_list **camobjs, t_list *objs)
{
	t_obj	*camobj;
	t_obj	*obj;

	while (objs)
	{
		obj = objs->content;
		ft_lstadd_back(camobjs, ft_lstnew(malloc(sizeof(*camobj))));
		camobj = ft_lstlast(*camobjs)->content;
		camobj->rot = obj->rot;
		camobj->outframe = obj->outframe;
		camobj->dots.rout = obj->dots.rout;
		camobj->dots.routsize = obj->dots.routsize;
		camobj->dots.dotsnum = obj->dots.dotsnum;
		camobj->dots.dots = obj->dots.dots;
		camobj->dots.pos = malloc(sizeof(*camobj->dots.pos)
			* (camobj->dots.dotsnum + CRNRS * (!camobj->dots.dotsnum)));
		camobj->dots.scale = obj->dots.scale;
		camobj->polys.poly = obj->polys.poly;
		camobj->polys.txtr = obj->polys.txtr;
		camobj->polys.polynum = obj->polys.polynum;
		camobj->colrs = obj->colrs;
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
	objtoobjpos(&camera->crdstm.pos, &camera->lightpos);
	quartrot(&camera->lightpos, &rot.axis);
	quartrot(&camera->lightpos, &rot.xyaxis);
	crsr = objs;
	camcrsr = camera->objs;
	while (crsr)
	{
		camobj = camcrsr->content;
		crdstmcopy(&objcast(crsr)->crdstm, &camobj->crdstm);
		objtoobjpos(&camera->crdstm.pos, &camobj->crdstm.pos);
		quartrot(&camobj->crdstm.pos, &rot.axis);
		quartrot(&camobj->crdstm.pos, &rot.xyaxis);
		crdstmrotbyaxis(&camobj->crdstm, &rot.axis, &rot.xyaxis);
		engine(&camobj->dots, &camobj->polys, &camobj->crdstm);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

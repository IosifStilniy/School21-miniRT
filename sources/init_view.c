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
		camobj->dots = obj->dots;
		camobj->dots.pos = malloc(sizeof(*camobj->dots.pos)
			* (camobj->dots.dotsnum + CRNRS * (!camobj->dots.dotsnum)));
		camobj->polys = obj->polys;
		if (!camobj->polys.polynum)
			camobj->polys.poly = malloc(sizeof(*camobj->polys.poly));
		camobj->colrs = obj->colrs;
		objs = objs->next;
	}
}

void	checkplanenorm(t_crdstm *plane, t_poly *poly, t_dots *dots)
{
	if (dots->dotsnum || plane->pos.z < 1)
		return ;
	poly->norm = plane->oz.vector;
	if (plane->oz.vector.z <= 0.001)
		return ;
	negativevector(&poly->norm);
	vectorsizing(1, &poly->norm, &poly->norm, NULL);
}

void	initview(t_list *objs, t_camera *camera, t_light *light)
{
	t_trans	trans;
	t_list	*crsr;
	t_list	*camcrsr;
	t_obj	*camobj;

	worldtocammatrix(trans.trans, trans.crdstm, trans.pos, &camera->crdstm);
	camera->lightpos = light->pos;
	transpos(&camera->lightpos, trans.trans);
	crsr = objs;
	camcrsr = camera->objs;
	while (crsr)
	{
		camobj = camcrsr->content;
		camobj->crdstm = objcast(crsr)->crdstm;
		transpos(&camobj->crdstm.pos, trans.trans);
		transpos(&camobj->crdstm.ox.vector, trans.crdstm);
		transpos(&camobj->crdstm.oy.vector, trans.crdstm);
		transpos(&camobj->crdstm.oz.vector, trans.crdstm);
		checkplanenorm(&camobj->crdstm, camobj->polys.poly, &camobj->dots);
		engine(&camobj->dots, &camobj->polys, &camobj->crdstm, &camobj->outframe);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

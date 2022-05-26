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
		camobj->move = obj->move;
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

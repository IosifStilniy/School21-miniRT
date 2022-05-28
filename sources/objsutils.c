#include "minirt.h"

void	insertinframeobj(t_list **inframes, t_list *objlst, t_cart *objpos)
{
	t_cart	*obj;
	t_axis	ref;
	t_list	*crsr;
	t_list	*prev;
	float	dstnc;

	vectorbuilder(objpos->x, objpos->y, objpos->z, &ref);
	dstnc = ref.length - objcast(objlst)->outframe;
	prev = NULL;
	crsr = *inframes;
	while (crsr)
	{
		obj = &objcast(crsr)->crdstm.pos;
		vectorbuilder(obj->x, obj->y, obj->z, &ref);
		if (dstnc < ref.length - objcast(crsr)->outframe)
			break ;
		prev = crsr;
		crsr = crsr->next;
	}
	if (prev)
		prev->next = objlst;
	else
		*inframes = objlst;
	objlst->next = crsr;
}

void	objexchanger(t_list *obj, t_list **dst, t_list **src, t_camobjs *collection)
{
	t_list	*prev;
	t_list	*crsr;

	crsr = *src;
	prev = NULL;
	while (crsr && crsr != obj)
	{
		prev = crsr;
		crsr = crsr->next;
	}
	if (!crsr)
		return ;
	if (prev)
		prev->next = crsr->next;
	else
		*src = crsr->next;
	if (collection->inframe == dst)
		insertinframeobj(&collection->inframe, obj, &objcast(obj)->crdstm.pos);
	else
		ft_lstadd_front(dst, obj);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:02:31 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 18:02:33 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	createcamobjs(t_list **camobjs, t_list *objs, t_cart **lightpos,
	int lightcount)
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
	*lightpos = malloc(sizeof(**lightpos) * lightcount);
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

void	objtrans(t_obj *camobj, t_obj *obj, t_trans *trans)
{
	camobj->crdstm = obj->crdstm;
	transpos(&camobj->crdstm.pos, trans->trans);
	transpos(&camobj->crdstm.ox.vector, trans->crdstm);
	transpos(&camobj->crdstm.oy.vector, trans->crdstm);
	transpos(&camobj->crdstm.oz.vector, trans->crdstm);
	checkplanenorm(&camobj->crdstm, camobj->polys.poly, &camobj->dots);
	engine(&camobj->dots, &camobj->polys, &camobj->crdstm, &camobj->outframe);
}

void	cornerbuilder(t_cart corners[CRNRS], t_res *wincntr, float focus)
{
	int	i;

	cartbuilder(0, 0, 1, &corners[0]);
	cartbuilder(-wincntr->x, -wincntr->y, focus, &corners[1]);
	cartbuilder(wincntr->x, -wincntr->y, focus, &corners[2]);
	cartbuilder(wincntr->x, wincntr->y, focus, &corners[3]);
	cartbuilder(-wincntr->x, wincntr->y, focus, &corners[4]);
	i = 0;
	while (++i < CRNRS)
		vectorsizing(1, &corners[i], &corners[i], NULL);
}

void	initview(t_list *objs, t_camera *camera, t_list *lights)
{
	t_trans	trans;
	t_list	*crsr;
	t_list	*camcrsr;
	int		i;

	worldtocammatrix(trans.trans, trans.crdstm, trans.pos, &camera->crdstm);
	i = -1;
	crsr = lights;
	while (crsr)
	{
		camera->lightpos[++i] = lightcast(crsr)->pos;
		transpos(&camera->lightpos[i], trans.trans);
		crsr = crsr->next;
	}
	crsr = objs;
	camcrsr = camera->objs;
	while (crsr)
	{
		objtrans(camcrsr->content, crsr->content, &trans);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

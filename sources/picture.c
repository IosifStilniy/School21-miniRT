#include "minirt.h"

void	createpic(t_camera *camera, t_res *winres)
{
	t_list	*polysinframe;
	int		pxlcount;
	int		*dephbuf;
	int		i;

	pxlcount = winres->x + winres->y;
	dephbuf = malloc(sizeof(*dephbuf) * pxlcount);
	i = -1;
	while (++i < pxlcount)
		dephbuf[i] = INT32_MAX;
		
}

void	createview(t_list *objs, t_camera *camera, t_res *wincntr, t_bool rotated)
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
		if (rotated)
		{
			crdstmcopy(&objcast(crsr)->crdstm, &camobj->crdstm);
			rotatedview(camobj, &rot, camera, wincntr);
		}
		else
			shiftedview(camobj, camera, wincntr);
		camcrsr = camcrsr->next;
		crsr = crsr->next;
	}
}

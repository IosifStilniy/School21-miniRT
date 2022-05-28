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

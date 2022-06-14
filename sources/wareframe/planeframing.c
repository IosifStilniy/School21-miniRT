#include "minirt.h"

t_bool	planeinframe(t_crdstm *plane, t_camera *camera)
{
	t_crdstm	cam;
	t_rot		rot;
	t_cart		corners[4];
	int			i;

	cartbuilder(-plane->pos.x, -plane->pos.y, -plane->pos.z, &cam.pos);
	cartbuilder(0, 0, 1, &cam.oz.vector);
	crdstmdefiner(&cam);
	objtoobjaxis(&cam, &plane, &rot);
	crdstmrotbyaxis(&cam, &rot.axis, &rot.xyaxis);
	i = -1;
	while (++i < 4)
		dotcrdstmtrnsltn(&camera->corners[i], &corners[i], 1, &cam);
}
#include "minirt.h"

void	crdstmrotbyaxis(t_crdstm *crdstm, t_axis *zaxis, t_axis *xyaxis)
{
	quartrot(&crdstm->oz.vector, zaxis);
	quartrot(&crdstm->oy.vector, zaxis);
	axisbuilder(&crdstm->oy.vector, &crdstm->oz.vector, &crdstm->ox);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
	if (!xyaxis)
		return ;
	quartrot(&crdstm->oz.vector, xyaxis);
	quartrot(&crdstm->oy.vector, xyaxis);
	axisbuilder(&crdstm->oy.vector, &crdstm->oz.vector, &crdstm->ox);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
}

void	objtoobjpos(t_cart *center, t_cart *dot)
{
	dot->x -= center->x;
	dot->y -= center->y;
	dot->z -= center->z;
}

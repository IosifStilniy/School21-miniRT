#include "minirt.h"

void	crdstmdefiner(t_crdstm *crdstm)
{
	t_axis	rot;

	vectorbuilder(1, 0, 0, &crdstm->ox);
	if (crdstm->ox.vector.x == crdstm->oz.vector.x)
	{
		vectorbuilder(0, 1, 0, &crdstm->ox);
		vectorbuilder(0, 0, 1, &crdstm->oy);
		return ;
	}
	axisbuilder(&crdstm->ox.vector, &crdstm->oz.vector, &rot);
	if (rot.ang < M_PI_2)
	{
		negativevector(&rot.vector);
		rot.ang = M_PI_2 - rot.ang;
	}
	else if (!comparef(rot.ang, M_PI, M_PI / 180))
		rot.ang -= M_PI_2;
	else
		vectorbuilder(0, 1, 0, &rot);
	quartrot(&crdstm->ox.vector, &rot);
	axisbuilder(&crdstm->oz.vector, &crdstm->ox.vector, &crdstm->oy);
}

float	sphereparser(char *str, t_obj *obj, char *prog)
{
	float	rad;

	rad = ft_atof(str) / 2.0f;
	str = skipnumnspaces(str);
	str = ft_get_color_values(str, obj->colrs, prog);
	vectorbuilder(1, 0, 0, &obj->crdstm.ox);
	vectorbuilder(0, 1, 0, &obj->crdstm.oy);
	vectorbuilder(0, 0, 1, &obj->crdstm.oz);
	return (spherebuilder(&obj->dots, &obj->polys, rad));
}

float	cylinderparser(char *str, t_obj *obj, char *prog)
{
	t_cart	norm;
	float	rad;
	float	height;

	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	vectorsizing(1, &obj->crdstm.oz.vector, &obj->crdstm.oz.vector,
		&obj->crdstm.oz.length);
	crdstmdefiner(&obj->crdstm);
	rad = ft_atof(str) / 2;
	str = skipnumnspaces(str);
	height = ft_atof(str);
	str = skipnumnspaces(str);
	ft_get_color_values(str, obj->colrs, prog);
	return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
}

void	planeparser(char *str, t_obj *obj, char *prog)
{
	t_cart	norm;

	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	vectorsizing(1, &obj->crdstm.oz.vector, &obj->crdstm.oz.vector,
		&obj->crdstm.oz.length);
	crdstmdefiner(&obj->crdstm);
	ft_get_color_values(str, obj->colrs, prog);
	obj->dots.dotsnum = 0;
	obj->polys.polynum = 0;
}

void	definevrtxsnorms(t_dots *dots, t_polys *polys)
{
	t_poly	*poly;
	int		i;

	dots->scale = malloc(sizeof(*dots->scale));
	cartbuilder(1, 1, 1, dots->scale);
	i = -1;
	while (++i < polys->polynum)
	{
		poly = &polys->poly[i];
		vectodot(&dots->dots[poly->dots[0]].norm, &poly->srcnorm);
		vectodot(&dots->dots[poly->dots[1]].norm, &poly->srcnorm);
		vectodot(&dots->dots[poly->dots[2]].norm, &poly->srcnorm);
	}	
	i = -1;
	while (++i < dots->dotsnum)
		vectorsizing(1, &dots->dots[i].norm, &dots->dots[i].norm, NULL);
}

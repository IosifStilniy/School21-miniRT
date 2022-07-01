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

float	sphereparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_data	*txtr;
	float	rad;

	rad = ft_atof(str) / 2.0f;
	str = skipnumnspaces(str, FALSE);
	str = ft_get_color_values(str, obj->colrs, prog);
	vectorbuilder(1, 0, 0, &obj->crdstm.ox);
	vectorbuilder(0, 1, 0, &obj->crdstm.oy);
	vectorbuilder(0, 0, 1, &obj->crdstm.oz);
	str = skipnumnspaces(str, TRUE);
	if (*str == '\n' || *str == '\0')
		return (spherebuilder(&obj->dots, &obj->polys, rad));
	str = getfilename(str, str + ft_strlen(str));
	txtr = &obj->polys.txtr;
	mlx_xpm_file_to_image(mlx, str, &txtr->res.x, &txtr->res.y);
	free(str);
	txtr->addr = mlx_get_data_addr(txtr->img, &txtr->bits_per_pixel, &txtr->line_length, &txtr->endian);
	return (spherebuilder(&obj->dots, &obj->polys, rad));
}

float	cylinderparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	t_data	*txtr;
	float	rad;
	float	height;

	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	vectorsizing(1, &obj->crdstm.oz.vector, &obj->crdstm.oz.vector,
		&obj->crdstm.oz.length);
	crdstmdefiner(&obj->crdstm);
	rad = ft_atof(str) / 2;
	str = skipnumnspaces(str, FALSE);
	height = ft_atof(str);
	str = skipnumnspaces(str, FALSE);
	str = ft_get_color_values(str, obj->colrs, prog);
	if (*str == '\n' || *str == '\0')
		return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
	str = getfilename(str, str + ft_strlen(str));
	txtr = &obj->polys.txtr;
	mlx_xpm_file_to_image(mlx, str, &txtr->res.x, &txtr->res.y);
	free(str);
	txtr->addr = mlx_get_data_addr(txtr->img, &txtr->bits_per_pixel, &txtr->line_length, &txtr->endian);
	return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
}

void	planeparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	t_data	*txtr;

	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	vectorsizing(1, &obj->crdstm.oz.vector, &obj->crdstm.oz.vector,
		&obj->crdstm.oz.length);
	crdstmdefiner(&obj->crdstm);
	str = ft_get_color_values(str, obj->colrs, prog);
	obj->dots.dotsnum = 0;
	obj->polys.polynum = 0;
	if (*str == '\n' || *str == '\0')
		return ;
	str = getfilename(str, str + ft_strlen(str));
	txtr = &obj->polys.txtr;
	mlx_xpm_file_to_image(mlx, str, &txtr->res.x, &txtr->res.y);
	free(str);
	txtr->addr = mlx_get_data_addr(txtr->img, &txtr->bits_per_pixel, &txtr->line_length, &txtr->endian);
}

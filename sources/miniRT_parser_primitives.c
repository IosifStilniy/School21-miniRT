/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser_primitives.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:03:22 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 20:04:46 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	txtrparsing(char *str, t_data *txtr, void *mlx)
{
	str = getfilename(str, str + ft_strlen(str));
	txtr->img = mlx_xpm_file_to_image(mlx, str, &txtr->res.x, &txtr->res.y);
	free(str);
	txtr->addr = mlx_get_data_addr(txtr->img, &txtr->bits_per_pixel,
			&txtr->line_length, &txtr->endian);
}

float	sphereparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	float	rad;

	rad = ft_atof(str) / 2.0f;
	str = skipnumnspaces(str, FALSE);
	str = ft_get_color_values(str, obj->colrs, prog);
	vectorbuilder(1, 0, 0, &obj->crdstm.ox);
	vectorbuilder(0, 1, 0, &obj->crdstm.oy);
	vectorbuilder(0, 0, 1, &obj->crdstm.oz);
	str = skipnumnspaces(str, TRUE);
	obj->polys.txtr.img = NULL;
	if (*str == '\n' || *str == '\0')
		return (spherebuilder(&obj->dots, &obj->polys, rad));
	txtrparsing(str, &obj->polys.txtr, mlx);
	return (spherebuilder(&obj->dots, &obj->polys, rad));
}

float	cylinderparser(char *str, t_obj *obj, char *prog, void *mlx)
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
	str = skipnumnspaces(str, FALSE);
	height = ft_atof(str);
	str = skipnumnspaces(str, FALSE);
	str = ft_get_color_values(str, obj->colrs, prog);
	obj->polys.txtr.img = NULL;
	if (*str == '\n' || *str == '\0')
		return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
	txtrparsing(str, &obj->polys.txtr, mlx);
	return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
}

void	planeparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;

	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	vectorsizing(1, &obj->crdstm.oz.vector, &obj->crdstm.oz.vector,
		&obj->crdstm.oz.length);
	crdstmdefiner(&obj->crdstm);
	str = ft_get_color_values(str, obj->colrs, prog);
	obj->dots.dotsnum = 0;
	obj->polys.polynum = 0;
	obj->polys.txtr.img = NULL;
	if (*str == '\n' || *str == '\0')
		return ;
	txtrparsing(str, &obj->polys.txtr, mlx);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser_primitives.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:03:22 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/05 17:35:29 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	hyperboloidparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	float	outframe;
	t_axis	rot;
	int		i;

	outframe = cylinderparser(str, obj, prog, mlx);
	vectorbuilder(0, 0, 1, &rot);
	rot.ang = M_PI_2;
	i = -1;
	while (++i < RNDSGMNTS)
		quartrot(&obj->dots.dots[i + 2], &rot);
	cylindersurfdefiner(obj->polys.poly - 1, obj->dots.dots,
		&obj->polys.txtr, obj->dots.dotsnum);
	definecylindervrtxs(obj->dots.dots, obj->polys.poly,
		obj->polys.polynum, &obj->polys.txtr);
	return (outframe);
}

float	coneparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	float	rad;
	float	height;

	str = ft_get_position_values(prog, str, &norm);
	if (comparef(vectorlength(&norm), 0, 0.001))
		customerr(prog, INVDEF, TRUE);
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
	obj->polys.checkerboard = FALSE;
	if (*str == '\n' || *str == '\0')
		return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
	txtrparsing(str, obj, mlx, prog);
	return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
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
	obj->polys.checkerboard = FALSE;
	if (*str == '\n' || *str == '\0')
		return (spherebuilder(&obj->dots, &obj->polys, rad));
	txtrparsing(str, obj, mlx, prog);
	return (spherebuilder(&obj->dots, &obj->polys, rad));
}

float	cylinderparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	float	rad;
	float	height;

	str = ft_get_position_values(prog, str, &norm);
	if (comparef(vectorlength(&norm), 0, 0.001))
		customerr(prog, INVDEF, TRUE);
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
	obj->polys.checkerboard = FALSE;
	if (*str == '\n' || *str == '\0')
		return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
	txtrparsing(str, obj, mlx, prog);
	return (cylinderbuilder(&obj->dots, &obj->polys, rad, height));
}

void	planeparser(char *str, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;

	str = ft_get_position_values(prog, str, &norm);
	if (comparef(vectorlength(&norm), 0, 0.001))
		customerr(prog, INVDEF, TRUE);
	ft_vectnorm(&norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	crdstmdefiner(&obj->crdstm);
	str = ft_get_color_values(str, obj->colrs, prog);
	obj->dots.dotsnum = 0;
	obj->polys.polynum = 0;
	obj->polys.txtr.img = NULL;
	obj->polys.checkerboard = FALSE;
	if (*str == '\n' || *str == '\0')
		return ;
	txtrparsing(str, obj, mlx, prog);
}

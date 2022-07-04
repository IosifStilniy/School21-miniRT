/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:13:01 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 21:51:05 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	importdotsandpolys(t_list *v, t_list *f, t_obj *obj)
{
	int		i;

	i = -1;
	while (++i < obj->dots.dotsnum)
	{
		obj->dots.dots[i] = *(t_cart *)v->content;
		v = v->next;
	}
	i = -1;
	while (++i < obj->polys.polynum)
	{
		obj->polys.poly[i] = *(t_poly *)f->content;
		obj->polys.poly[i].txtr = &obj->polys.txtr;
		f = f->next;
	}
}

static void	importobj(int fd, t_obj *obj)
{
	t_import	imp;

	imp.v = NULL;
	imp.vn = NULL;
	imp.vt = NULL;
	imp.f = NULL;
	modelparser(fd, &imp);
	close(fd);
	obj->dots.dotsnum = ft_lstsize(imp.v);
	obj->dots.dots = malloc(sizeof(*obj->dots.dots) * obj->dots.dotsnum);
	obj->polys.polynum = ft_lstsize(imp.f);
	obj->polys.poly = malloc(sizeof(*obj->polys.poly) * obj->polys.polynum);
	importdotsandpolys(imp.v, imp.f, obj);
	ft_lstclear(&imp.v, &free);
	ft_lstclear(&imp.f, &free);
}

char	*readfile(char *line, int *fd, char *prog)
{
	char	*bound;
	char	*file;

	line = skipnumnspaces(line, TRUE);
	bound = line;
	while (!ft_strchr(SPACES, *bound))
		bound++;
	file = getfilename(line, bound);
	*fd = file_check(file, prog, FALSE);
	free(file);
	return (bound);
}

static float	getoutframe(t_cart *dots, int dotnum, t_cart *scale)
{
	float	outframe;
	float	length;
	int		i;

	centroiddefiner(dots, dotnum);
	outframe = 0;
	i = -1;
	while (++i < dotnum)
	{
		length = vectorlength(&dots[i]);
		if (length > outframe)
			outframe = length;
	}
	cartbuilder(1, 1, 1, scale);
	if (outframe < 10)
	{
		cartbuilder(10 / outframe, 10 / outframe, 10 / outframe, scale);
		outframe = 10;
	}
	return (outframe);
}

float	objparser(char *line, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	int		fd;

	line = readfile(line, &fd, prog);
	importobj(fd, obj);
	line = ft_get_position_values(prog, line, &obj->crdstm.pos);
	line = ft_get_position_values(prog, line, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &obj->crdstm.oz);
	crdstmdefiner(&obj->crdstm);
	obj->colrs = malloc(sizeof(*obj->colrs));
	line = ft_get_color_values(line, obj->colrs, prog);
	obj->dots.scale = malloc(sizeof(*obj->dots.scale));
	obj->polys.txtr.img = NULL;
	line = skipnumnspaces(line, TRUE);
	obj->polys.txtr.img = NULL;
	if (*line == '\n' || *line == '\0')
		return (getoutframe(obj->dots.dots, obj->dots.dotsnum,
				obj->dots.scale));
	txtrparsing(line, &obj->polys.txtr, mlx, &obj->polys.checkerboard);
	return (getoutframe(obj->dots.dots, obj->dots.dotsnum, obj->dots.scale));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modelparser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:13:25 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 19:05:18 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*vrtxparser(char *line, t_list *vt, t_list *vn, t_vrtx *vrtx)
{
	vrtx->dot = ft_atoi(line) - 1;
	line = skipnumnspaces(line, FALSE);
	if (ft_atoi(++line))
		vrtx->uv = *(getcart(vt, ft_atoi(line)));
	line = skipnumnspaces(line, FALSE);
	vrtx->srcnorm = *(getcart(vn, ft_atoi(++line)));
	line = skipnumnspaces(line, FALSE);
	return (line);
}

static void	interpolatednorm(t_cart *norm, t_vrtx vrtxs[3], t_list *dotlist)
{
	t_vrtx	buf;
	float	scalar;

	normbuilder(getcart(dotlist, vrtxs[0].dot + 1),
		getcart(dotlist, vrtxs[1].dot + 1),
		getcart(dotlist, vrtxs[2].dot + 1),
		norm);
	checkvrtxsnorms(vrtxs, norm);
	ft_dotprod(&vrtxs[0].srcnorm, norm, &scalar);
	if (scalar >= 0)
		return ;
	buf = vrtxs[2];
	vrtxs[2] = vrtxs[1];
	vrtxs[1] = buf;
	normbuilder(getcart(dotlist, vrtxs[0].dot + 1),
		getcart(dotlist, vrtxs[1].dot + 1),
		getcart(dotlist, vrtxs[2].dot + 1),
		norm);
}

static void	facefiller(t_import *imp, char *line, t_bool *interpolate)
{
	t_poly	*poly;
	t_poly	*newpoly;

	poly = malloc(sizeof(*poly));
	line = avoidonelinevrtxs(poly->vrtxs, line, imp);
	poly->interpolate = *interpolate;
	interpolatednorm(&poly->srcnorm, poly->vrtxs, imp->v);
	ft_lstadd_front(&imp->f, ft_lstnew(poly));
	while (notendedline(line))
	{
		newpoly = malloc(sizeof(*newpoly));
		newpoly->vrtxs[0] = poly->vrtxs[0];
		newpoly->vrtxs[1] = poly->vrtxs[2];
		line = vrtxparser(line, imp->vt, imp->vn, &newpoly->vrtxs[2]);
		newpoly->interpolate = *interpolate;
		newpoly->srcnorm = poly->srcnorm;
		ft_lstadd_front(&imp->f, ft_lstnew(newpoly));
		poly = newpoly;
	}
	*interpolate = FALSE;
}

static void	vrtxfiller(t_list **v, char *line, t_bool normilize, t_bool txtr)
{
	t_cart	*vertex;

	vertex = malloc(sizeof(*vertex));
	vertex->x = ft_atof(line);
	line = skipnumnspaces(line, FALSE);
	vertex->y = ft_atof(line);
	line = skipnumnspaces(line, FALSE);
	vertex->z = ft_atof(line);
	if (normilize && vectorlength(vertex) > 0)
		vectorsizing(1, vertex, vertex, NULL);
	if (txtr)
	{
		vertex->x += (vertex->x < 0);
		vertex->y += (vertex->y < 0);
	}
	ft_lstadd_back(v, ft_lstnew(vertex));
}

void	modelparser(int fd, t_import *imp)
{
	char	*line;
	t_bool	interpolate;

	interpolate = FALSE;
	line = get_next_line(fd);
	while (line)
	{
		if (*line != 'v' && *line != 'f' && *line != 's')
			;
		else if (!ft_strncmp(line, "v ", 2))
			vrtxfiller(&imp->v, line + 2, FALSE, FALSE);
		else if (!ft_strncmp(line, "vn ", 3))
			vrtxfiller(&imp->vn, line + 3, TRUE, FALSE);
		else if (!ft_strncmp(line, "vt ", 3))
			vrtxfiller(&imp->vt, line + 3, FALSE, TRUE);
		else if (!ft_strncmp(line, "s ", 2))
			interpolate++;
		else if (!ft_strncmp(line, "f ", 2))
			facefiller(imp, line + 2, &interpolate);
		free(line);
		line = get_next_line(fd);
	}
	ft_lstclear(&imp->vn, &free);
	ft_lstclear(&imp->vt, &free);
}

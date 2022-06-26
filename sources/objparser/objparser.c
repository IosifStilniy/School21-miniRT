#include "minirt.h"

t_cart	*getcart(t_list *v, int indx)
{
	int	i;

	i = 0;
	while (++i < indx && v)
		v = v->next;
	return (v->content);
}

char	*vrtxparser(char *line, t_list *vt, t_list *vn, t_vrtx *vrtx)
{
	int	indx;

	vrtx->dot = ft_atoi(line);
	line = skipnumnspaces(line);
	if (ft_atoi(++line))
		vrtx->uv = *(getcart(vt, ft_atoi(line)));
	line = skipnumnspaces(line);
	vrtx->srcnorm = *(getcart(vn, ft_atoi(++line)));
	line = skipnumnspaces(line);
	return (line);
}

void	interpolatednorm(t_cart *norm, t_vrtx vrtxs[3])
{
	t_cart	vns[3];

	vectorsizing(1.f / 3, &vrtxs[0].srcnorm, &vns[0], NULL);
	vectorsizing(1.f / 3, &vrtxs[1].srcnorm, &vns[1], NULL);
	vectorsizing(1.f / 3, &vrtxs[2].srcnorm, &vns[2], NULL);
	cartbuilder(0, 0, 0, norm);
	vectodot(norm, &vns[0]);
	vectodot(norm, &vns[1]);
	vectodot(norm, &vns[2]);
	vectorsizing(1, norm, norm, NULL);
}

char	*notendedline(char *line)
{
	while (ft_strchr(SPACES, *line))
		++line;
	return (ft_strchr("123456789", *line));
}

void	facefiller(t_import *imp, char *line, t_bool *interpolate)
{
	t_cart	dots[3];
	t_poly	*poly;
	t_poly	*newpoly;

	poly = malloc(sizeof(*poly));
	line = vrtxparser(line, imp->vt, imp->vn, &poly->vrtxs[0]);
	line = vrtxparser(line, imp->vt, imp->vn, &poly->vrtxs[1]);
	line = vrtxparser(line, imp->vt, imp->vn, &poly->vrtxs[2]);
	poly->interpolate = *interpolate;
	interpolatednorm(&poly->srcnorm, poly->vrtxs);
	ft_lstadd_front(&imp->f, ft_lstnew(poly));
	while (notendedline(line))
	{
		newpoly = malloc(sizeof(*newpoly));
		newpoly->vrtxs[0] = poly->vrtxs[0];
		newpoly->vrtxs[1] = poly->vrtxs[2];
		line = vrtxparser(line, imp->vt, imp->vn, &newpoly->vrtxs[2]);
		newpoly->interpolate = *interpolate;
		newpoly->srcnorm = poly->srcnorm;
		ft_lstadd_front(&imp->f, ft_lstnew(poly));
		poly = newpoly;
	}
	*interpolate = FALSE;
}

void	vrtxfiller(t_list **v, char *line)
{
	t_cart	*vertex;

	vertex = malloc(sizeof(*vertex));
	vertex->x = ft_atof(line);
	line = skipnumnspaces(line);
	vertex->y = ft_atof(line);
	line = skipnumnspaces(line);
	vertex->z = ft_atof(line);
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
		if (*line != 'v' || *line != 'f')
			;
		else if (!ft_strncmp(line, "v ", 2))
			vrtxfiller(&imp->v, line + 2);
		else if (!ft_strncmp(line, "vn ", 3))
			vrtxfiller(&imp->vn, line + 3);
		else if (!ft_strncmp(line, "vt ", 3))
			vrtxfiller(&imp->vt, line + 3);
		else if (!ft_strncmp(line, "s ", 2))
			interpolate++;
		else if (!ft_strncmp(line, "f ", 2))
			facefiller(&imp->f, line + 2, &interpolate);
		free(line);
		line = get_next_line(fd);
	}
}

t_obj	*objparser(char *model, char *txtr, char *heightmap, t_info *info)
{
	t_import	imp;
	t_obj		*obj;
	int			fd;

	imp.v = NULL;
	imp.vn = NULL;
	imp.vt = NULL;
	imp.f = NULL;
	fd = file_check(model, info->prog);
	modelparser(fd, &imp);
	obj = malloc(sizeof(*obj));
	obj->polys.txtr->img = NULL;
	if (txtr)
	{
		mlx_xpm_file_to_image(info->mlx_ptr, txtr, &obj->polys.txtr->res.x, &obj->polys.txtr->res.y);
		obj->polys.txtr->addr = mlx_get_data_addr(obj->polys.txtr->img, &obj->polys.txtr->bits_per_pixel, &obj->polys.txtr->line_length, &obj->polys.txtr->endian);
	}
	obj->polys.heightmap->img = NULL;
	if (heightmap)
	{
		mlx_xpm_file_to_image(info->mlx_ptr, heightmap, &obj->polys.heightmap->res.x, &obj->polys.heightmap->res.y);
		obj->polys.txtr->addr = mlx_get_data_addr(obj->polys.heightmap->img, &obj->polys.heightmap->bits_per_pixel, &obj->polys.heightmap->line_length, &obj->polys.heightmap->endian);
	}
}
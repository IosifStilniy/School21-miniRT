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

static float	getoutframe(t_cart *dots, int dotnum)
{
	float	outframe;
	float	length;
	int		i;

	outframe = 0;
	i = -1;
	while (++i < dotnum)
	{
		length = vectorlength(&dots[i]);
		if (length > outframe)
			outframe = length;
	}
	return (outframe);
}

float	objparser(char *line, t_obj *obj, char *prog, void *mlx)
{
	t_cart	norm;
	t_data	*txtr;
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
	cartbuilder(1, 1, 1, obj->dots.scale);
	obj->polys.txtr.img = NULL;
	line = skipnumnspaces(line, TRUE);
	if (*line == '\n' || *line == '\0')
		return getoutframe(obj->dots.dots, obj->dots.dotsnum);
	line = getfilename(line, line + ft_strlen(line));
	txtr = &obj->polys.txtr;
	mlx_xpm_file_to_image(mlx, line, &txtr->res.x, &txtr->res.y);
	free(line);
	txtr->addr = mlx_get_data_addr(txtr->img, &txtr->bits_per_pixel, &txtr->line_length, &txtr->endian);
	return (getoutframe(obj->dots.dots, obj->dots.dotsnum));
}

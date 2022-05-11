/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:21:33 by ncarob            #+#    #+#             */
/*   Updated: 2022/05/11 21:54:54 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	light_definition(char *line, t_light *a_light, t_light *lights, char *prog)
{
	t_light	*light;

	if ((*line == 'A' && a_light->determined) || (*line == 'L' && lights->determined))
		customerr(prog, DUPDET, TRUE);
	light = a_light;
	if (*line++ == 'L')
		light = lights;
	light->determined = TRUE;
	while (ft_strchr(SPACES, *line))
		line++;
	if (!ft_strchr("01.", *line))
		customerr(prog, INVDEF, TRUE);
	light->light_ratio = ft_atof(line);
	if (!(0 <= light->light_ratio && light->light_ratio <= 1))
		customerr(prog, INVDEF, TRUE);
	if (light == &lights)
		line = ft_get_position_values(prog, ++line, &lights->pos);
	line = ft_get_color_values(line, &light->color, prog);
	while (ft_strchr(NUMSPACES, *line))
		line++;
	if (*line != '\n' || *line)
		customerr(prog, INVDEF, TRUE);
}

static void	ft_fill_camera_info(char *str, t_camera *camera, t_rot *rot, char *prog)
{
	t_cart	norm;

	if (camera->determined)
		customerr(prog, DUPDET, TRUE);
	str = ft_get_position_values(str, &camera->pos, prog);
	str = ft_get_position_values(str, &norm, prog);
	vectorbuilder(norm.x, norm.y, norm.z, &camera->crdstm.oz);
	if (camera->crdstm.oz.length != 1)
		customerr(prog, INVDEF, TRUE);
	crdstmdefiner(&camera->crdstm);
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("0123456789", *str))
		customerr(prog, INVDEF, TRUE);
	camera->fov = ft_atoi(str);
	if (!(0 <= camera->fov  && camera->fov <= 180))
		customerr(prog, INVDEF, TRUE);
	while (ft_strchr("0123456789", *str))
		str++;
	while (ft_strchr(SPACES, *str))
		str++;
	if (*str != '\n' || *str)
		customerr(prog, INVDEF, TRUE);
	camera->rot = rot;
}

static void	primitivebuilder(char *str, t_list **objs, char *prog, t_rot *rot)
{
	int		i;

	while (ft_strchr(SPACES, *str))
		str++;
	i = 0;
	while (i < NUMPRMTVS)
		if (!ft_strncmp(PRMTVS + i * 2, str, 2) || (++i && FALSE))
			break ;
	if (i == NUMPRMTVS)
		customerr(prog, INVDEF, TRUE);
	ft_lstadd_front(objs, ft_lstnew(malloc(sizeof(t_obj))));
	objcast(*objs)->rot = rot;
	objcast(*objs)->polys.polynorms = NULL;
	str = ft_get_position_values(prog, str, &objcast(*objs)->crdstm.pos);
	if (!i)
		objcast(*objs)->outframe = sphereparser(str, (*objs)->content, prog);
	else if (i == 1)
		planeparser(str, (*objs)->content, prog);
	else if (i == 2)
		objcast(*objs)->outframe = cylinderparser(str, (*objs)->content, prog);
	objcast(*objs)->dots.scale = 1;
	rot->end = &objcast(*objs)->crdstm.oz;
	objrot(*objs, rot, &objcast(*objs)->crdstm.oz);
}

void	ft_read_information(int fd, t_info *info)
{
	char	*line;
	char	*crsr;

	info->objects = NULL;
	line = get_next_line(fd);
	while (line)
	{
		crsr = line;
		while (*crsr && *crsr != '\n' && ft_strchr(SPACES, *crsr))
			crsr++;
		if (ft_strchr("AL", *crsr))
			light_definition(crsr, &info->a_light, &info->lights, info->prog);
		else if (*crsr == 'C')
			ft_fill_camera_info(++crsr, &info->win.camera, &info->rot, info->prog);
		else if (*crsr && *crsr != '\n')
			primitivesbuilder(crsr, &info->objects, info->prog, &info->rot);
		free(line);
		line = get_next_line(fd);
	}
	if (!(info->win.camera.determined * info->lights.determined * info->a_light.determined))
		customerr(info->prog, "undefined camera and/or lights", TRUE);
	info->win.camera.focus = info->win.cntr.x / tanf(info->win.camera.fov / 360 * M_PI) / 2;
}

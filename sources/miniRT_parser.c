/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:21:33 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/29 20:52:36 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	light_definition(char *line, t_light *a_light, t_list **lights, char *prog)
{
	t_light	*light;
	t_list	*new;
	
	new = NULL;
	if (*line++ == 'L')
	{
		light = (t_light *)malloc(sizeof(t_light));
		if (!light)
			customerr(prog, ERR_TEXT, TRUE);
		new = ft_lstnew(light);
		ft_lstadd_back(lights, new);
	}
	else
		light = a_light;
	light->determined = TRUE;
	while (ft_strchr(SPACES, *line))
		line++;
	if (new)
		line = ft_get_position_values(prog, line, &((t_light *)(new->content))->pos);
	while (ft_strchr(SPACES, *line))
		line++;
	if (!ft_strchr("+01.", *line))
		customerr(prog, INVDEF, TRUE);
	light->light_ratio = ft_atof(line);
	if (!(0 <= light->light_ratio && light->light_ratio <= 1))
		customerr(prog, INVDEF, TRUE);
	while (*line && ft_strchr("0123456789.", *line))
		line++;
	line = ft_get_color_values(line, &light->color, prog);
	while (ft_strchr(SPACES, *line))
		line++;
	if (*line != '\n' && *line)
		customerr(prog, INVDEF, TRUE);
}

static void	ft_fill_camera_info(char *str, t_camera *camera, t_rot *rot, char *prog)
{
	t_cart	norm;

	if (camera->determined++)
		customerr(prog, DUPDET, TRUE);
	str = ft_get_position_values(prog, str, &camera->crdstm.pos);
	str = ft_get_position_values(prog, str, &norm);
	vectorbuilder(norm.x, norm.y, norm.z, &camera->crdstm.oz);
	vectorsizing(1, &camera->crdstm.oz.vector, &camera->crdstm.oz.vector, &camera->crdstm.oz.length);
	crdstmdefiner(&camera->crdstm);
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("0123456789", *str))
		customerr(prog, INVDEF, TRUE);
	camera->fov = ft_atoi(str) * M_PI / 360;
	if (!(-0.001 <= camera->fov && camera->fov <= 90.001))
		customerr(prog, INVDEF, TRUE);
	while (ft_strchr("0123456789", *str))
		str++;
	while (ft_strchr(SPACES, *str))
		str++;
	if (*str != '\n' && *str)
		customerr(prog, INVDEF, TRUE);
	camera->rot = rot;
	camera->lightpos = NULL;	
}

static void	primitivesbuilder(char *str, t_list **objs, char *prog, t_rot *rot)
{
	int		i;
	t_obj	*obj;

	while (ft_strchr(SPACES, *str))
		str++;
	i = -1;
	while (++i < NUMPRMTVS)
		if (!ft_strncmp(&PRMTVS[2 * i], str, 2))
			break ;
	if (i == NUMPRMTVS)
		customerr(prog, INVDEF, TRUE);
	str += 2;
	obj = malloc(sizeof(*obj));
	ft_lstadd_front(objs, ft_lstnew(obj));
	obj->rot = rot;
	str = ft_get_position_values(prog, str, &obj->crdstm.pos);
	obj->colrs = malloc(sizeof(*obj->colrs));
	if (!i)
		obj->outframe = sphereparser(str, obj, prog);
	else if (i == 1)
		planeparser(str, obj, prog);
	else if (i == 2)
		obj->outframe = cylinderparser(str, obj, prog);
	definevrtxsnorms(&obj->dots, &obj->polys);
}

void	definecamera(t_camera *camera, t_res *wincntr)
{
	int	i;

	camera->focus = wincntr->x / tanf(camera->fov);
	if (camera->focus < 1)
		camera->focus = 1;
	camera->attached.obj = NULL;
	cartbuilder(0, 0, 1, &camera->corners[0]);
	cartbuilder(-wincntr->x, -wincntr->y, camera->focus, &camera->corners[1]);
	cartbuilder(wincntr->x, -wincntr->y, camera->focus, &camera->corners[2]);
	cartbuilder(wincntr->x, wincntr->y, camera->focus, &camera->corners[3]);
	cartbuilder(-wincntr->x, wincntr->y, camera->focus, &camera->corners[4]);
	i = 0;
	while (++i < CRNRS)
		vectorsizing(1, &camera->corners[i], &camera->corners[i], NULL);
}

void	ft_read_information(int fd, t_info *info)
{
	char	*line;
	char	*crsr;

	info->objects = NULL;
	info->lights = NULL;
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
	if (!info->lights->content || !(info->win.camera.determined * ((t_light *)(info->lights->content))->determined * info->a_light.determined))
		customerr(info->prog, "undefined camera and/or lights", TRUE);
	definecamera(&info->win.camera, &info->win.cntr);
}

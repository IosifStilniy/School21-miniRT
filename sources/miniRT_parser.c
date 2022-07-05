/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:21:33 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 21:28:37 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	light_definition(char *line, t_light *a_light, t_list **lights,
	char *prog)
{
	t_light	*light;

	light = a_light;
	if (*line++ == 'L')
	{
		ft_lstadd_front(lights, ft_lstnew(malloc(sizeof(*light))));
		light = (*lights)->content;
	}
	line = skipnumnspaces(line, TRUE);
	if (light != a_light)
		line = ft_get_position_values(prog, line, &light->pos);
	line = skipnumnspaces(line, TRUE);
	if (!ft_strchr("+01.", *line))
		customerr(prog, INVDEF, TRUE);
	light->light_ratio = ft_atof(line);
	if (!(0 <= light->light_ratio && light->light_ratio <= 1))
		customerr(prog, INVDEF, TRUE);
	line = skipnumnspaces(line, FALSE);
	line = ft_get_color_values(line, &light->color, prog);
	line = skipnumnspaces(line, TRUE);
	if (*line != '\n' && *line)
		customerr(prog, INVDEF, TRUE);
}

static void	ft_fill_camera_info(char *str, t_list **cameras, char *prog)
{
	t_camera	*camera;
	t_cart		norm;

	camera = malloc(sizeof(*camera));
	str = ft_get_position_values(prog, str, &camera->crdstm.pos);
	str = ft_get_position_values(prog, str, &norm);
	if (comparef(vectorlength(&norm), 0, 0.001))
		customerr(prog, INVDEF, TRUE);
	vectorbuilder(norm.x, norm.y, norm.z, &camera->crdstm.oz);
	vectorsizing(1, &camera->crdstm.oz.vector, &camera->crdstm.oz.vector,
		&camera->crdstm.oz.length);
	crdstmdefiner(&camera->crdstm);
	str = skipnumnspaces(str, TRUE);
	if (!ft_strchr("0123456789", *str))
		customerr(prog, INVDEF, TRUE);
	camera->fov = ft_atoi(str) * M_PI / 360;
	if (!(-0.001 <= camera->fov && camera->fov <= 90.001))
		customerr(prog, INVDEF, TRUE);
	str = skipnumnspaces(str, FALSE);
	if (*str != '\n' && *str)
		customerr(prog, INVDEF, TRUE);
	ft_lstadd_back(cameras, ft_lstnew(camera));
}

static char	*definecameras(t_camera **wincam, t_list *cameras, t_res *wincntr)
{
	t_camera	*camera;
	char		*buf;
	char		*camtxt;
	int			camcount;

	*wincam = cameras->content;
	camcount = ft_lstsize(cameras);
	while (cameras)
	{
		camera = cameras->content;
		camera->focus = wincntr->x / tanf(camera->fov);
		if (camera->focus < 1)
			camera->focus = 1;
		camera->attached.obj = NULL;
		cornerbuilder(camera->corners, wincntr, camera->focus);
		cameras = cameras->next;
	}
	buf = ft_itoa(camcount);
	camtxt = ft_strjoin("Camera: 1 of ", buf);
	free(buf);
	return (camtxt);
}

void	ft_read_information(int fd, t_info *info)
{
	char	*line;
	char	*crsr;
	t_bool	dets[2];

	ft_bzero(dets, sizeof(*dets) * 2);
	line = get_next_line(fd);
	while (line)
	{
		crsr = line;
		while (*crsr && *crsr != '\n' && ft_strchr(SPACES, *crsr))
			crsr++;
		if (((*crsr == 'A' && ++dets[0]) || *crsr == 'L'))
			light_definition(crsr, &info->a_light, &info->lights, info->prog);
		else if (*crsr == 'C' && ++dets[1])
			ft_fill_camera_info(++crsr, &info->win.cameras, info->prog);
		else if (*crsr && *crsr != '\n')
			primitivesbuilder(crsr, &info->objects, info, &info->rot);
		free(line);
		line = get_next_line(fd);
	}
	if (dets[0] != 1 || dets[1] != 1)
		customerr(info->prog, DUPDET, TRUE);
	info->camtext = definecameras(&info->win.camera, info->win.cameras,
			&info->win.cntr);
}

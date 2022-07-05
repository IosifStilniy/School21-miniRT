/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objparser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:13:11 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/05 15:03:13 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_cart	*getcart(t_list *v, int indx)
{
	int	i;

	i = 0;
	while (++i < indx && v)
		v = v->next;
	return (v->content);
}

char	*notendedline(char *line)
{
	while (ft_strchr(SPACES, *line))
		++line;
	return (ft_strchr("123456789", *line));
}

char	*getfilename(char *start, char *end, char *prog, t_bool txtr)
{
	char	*file;
	int		i;

	file = malloc(sizeof(*file) * (end - start + 1));
	i = -1;
	while (start + ++i < end)
		file[i] = *(start + i);
	file[i] = '\0';
	if (file[i - 1] == '\n')
		file[i - 1] = '\0';
	if (txtr && (ft_strlen(file) < 4 || ft_strncmp(".xpm",
				file + ft_strlen(file) - 4, 3)))
		customerr(prog, "needs file extension .xpm for textures\n", TRUE);
	else if (!txtr && (ft_strlen(file) < 4 || ft_strncmp(".obj",
				file + ft_strlen(file) - 4, 3)))
		customerr(prog, "needs file extension .obj for 3D-model\n", TRUE);
	return (file);
}

void	centroiddefiner(t_cart *dots, int dotsnum)
{
	t_cart	center;
	int		i;

	cartbuilder(0, 0, 0, &center);
	i = -1;
	while (++i < dotsnum)
	{
		center.x += dots[i].x;
		center.y += dots[i].y;
		center.z += dots[i].z;
	}
	vectorsizing(vectorlength(&center) / dotsnum, &center, &center, NULL);
	i = -1;
	while (++i < dotsnum)
	{
		dots[i].x -= center.x;
		dots[i].y -= center.y;
		dots[i].z -= center.z;
	}
}

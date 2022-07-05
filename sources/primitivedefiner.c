/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitivedefiner.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 14:57:34 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 14:58:55 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	primitivedefiner(char *str)
{
	int	i;

	i = -1;
	while (++i < NUMPRMTVS)
		if (!ft_strncmp(&PRMTVS[2 * i], str, 2))
			break ;
	return (i);
}

static void	primitivedistributor(int i, char *str, t_obj *obj, t_info *info)
{
	if (!i)
		obj->outframe = sphereparser(str, obj, info->prog, info->mlx_ptr);
	else if (i == 1)
		planeparser(str, obj, info->prog, info->mlx_ptr);
	else if (i == 2)
		obj->outframe = cylinderparser(str, obj, info->prog, info->mlx_ptr);
	else if (i == 3)
		obj->outframe = coneparser(str, obj, info->prog, info->mlx_ptr);
	else if (i == 4)
		obj->outframe = hyperboloidparser(str, obj, info->prog, info->mlx_ptr);
}

void	primitivesbuilder(char *str, t_list **objs, t_info *info,
	t_rot *rot)
{
	int		i;
	t_obj	*obj;

	str = skipnumnspaces(str, TRUE);
	obj = malloc(sizeof(*obj));
	ft_lstadd_front(objs, ft_lstnew(obj));
	obj->rot = rot;
	i = primitivedefiner(str);
	if (i == NUMPRMTVS)
	{
		obj->outframe = objparser(str, obj, info->prog, info->mlx_ptr);
		return ;
	}
	str += 2;
	str = ft_get_position_values(info->prog, str, &obj->crdstm.pos);
	obj->colrs = malloc(sizeof(*obj->colrs));
	obj->dots.scale = malloc(sizeof(*obj->dots.scale));
	cartbuilder(1, 1, 1, obj->dots.scale);
	primitivedistributor(i, str, obj, info);
}

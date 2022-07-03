/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   framerouting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:09:18 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 18:10:17 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_bool	checkrout(int dot1, int dot2, t_list *routs)
{
	t_bool	exist;
	int		*rout;

	while (routs)
	{
		rout = routs->content;
		exist = (rout[0] == dot1 || rout[0] == dot2);
		exist *= (rout[1] == dot1 || rout[1] == dot2);
		if (exist)
			return (TRUE);
		routs = routs->next;
	}
	return (FALSE);
}

void	createroutlstelem(int dot1, int dot2, t_list **routs)
{
	int		*rout;

	rout = malloc(sizeof(*rout) * 2);
	rout[0] = dot1;
	rout[1] = dot2;
	ft_lstadd_front(routs, ft_lstnew(rout));
}

void	createroutlist(t_poly *polys, int polynum, t_list **routs)
{
	int	dots[3];
	int	i;

	i = -1;
	while (++i < polynum)
	{
		dots[0] = polys[i].vrtxs[0].dot;
		dots[1] = polys[i].vrtxs[1].dot;
		dots[2] = polys[i].vrtxs[2].dot;
		if (!checkrout(dots[0], dots[1], *routs))
			createroutlstelem(dots[0], dots[1], routs);
		if (!checkrout(dots[1], dots[2], *routs))
			createroutlstelem(dots[1], dots[2], routs);
		if (!checkrout(dots[2], dots[0], *routs))
			createroutlstelem(dots[2], dots[0], routs);
	}
}

int	lsttointarr(t_list *routs, int (**routarr)[2])
{
	int	size;
	int	i;
	int	*rout;

	size = ft_lstsize(routs);
	*routarr = malloc(sizeof(int [2]) * size);
	i = -1;
	while (++i < size)
	{
		rout = routs->content;
		(*routarr)[i][0] = rout[0];
		(*routarr)[i][1] = rout[1];
		routs = routs->next;
	}
	return (size);
}

void	createframerouts(t_list *objs)
{
	t_list	*routs;
	t_obj	*obj;

	while (objs)
	{
		obj = objs->content;
		routs = NULL;
		obj->dots.rout = NULL;
		if (!obj->dots.dotsnum)
		{
			objs = objs->next;
			continue ;
		}
		createroutlist(obj->polys.poly, obj->polys.polynum, &routs);
		obj->dots.routsize = lsttointarr(routs, &obj->dots.rout);
		ft_lstclear(&routs, &free);
		objs = objs->next;
	}
}

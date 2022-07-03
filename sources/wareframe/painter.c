/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   painter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:08:45 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/03 18:09:07 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	framepainter(t_dots *dots, float focus, t_data *img, t_ui color)
{
	t_cart	paintdots[2];
	int		i;

	i = -1;
	while (++i < dots->routsize)
	{
		paintdots[0] = dots->pos[dots->rout[i][0]];
		paintdots[1] = dots->pos[dots->rout[i][1]];
		paintline(paintdots, color, focus, img);
	}
}

void	normpainter(t_obj *obj, float focus, t_data *img)
{
	t_cart	paintdots[2];
	float	lngth;
	int		i;
	int		j;

	lngth = 0.05 * obj->outframe;
	i = -1;
	while (++i < obj->polys.polynum)
	{
		j = -1;
		while (++j < 3)
		{
			if (vectorlength(&obj->polys.poly[i].vrtxs[j].norm) < 0.999)
				continue ;
			vectorsizing(lngth, &obj->polys.poly[i].vrtxs[j].norm,
				&paintdots[0], NULL);
			paintdots[1] = obj->dots.pos[obj->polys.poly[i].vrtxs[j].dot];
			vectodot(&paintdots[0], &paintdots[1]);
			paintline(paintdots, NORMCLR, focus, img);
		}
	}
}

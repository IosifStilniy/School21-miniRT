/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vrtxchecker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:12:06 by dcelsa            #+#    #+#             */
/*   Updated: 2022/07/04 21:21:48 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	repeatvrtxnorm(t_vrtx *vrtx, t_vrtx vrtxs[3])
{
	int	i;

	i = -1;
	while (++i < 3)
	{
		if (vrtx == &vrtxs[i] || vectorlength(&vrtxs[i].srcnorm) < 0.999)
			continue ;
		vrtx->srcnorm = vrtxs[i].srcnorm;
	}
}

void	checkvrtxsnorms(t_vrtx vrtxs[3], t_cart *polynorm)
{
	float	sumlength;
	int		i;
	int		j;

	sumlength = vectorlength(&vrtxs[0].srcnorm)
		+ vectorlength(&vrtxs[1].srcnorm) + vectorlength(&vrtxs[2].srcnorm);
	i = -1;
	while (++i < 3)
	{
		j = -1;
		if (comparef(vectorlength(&vrtxs[i].srcnorm), 1, 0.001))
			continue ;
		else if (sumlength < 0.999)
			vrtxs[i].srcnorm = *polynorm;
		else
			repeatvrtxnorm(&vrtxs[i], vrtxs);
	}
}

char	*avoidonelinevrtxs(t_vrtx vrtxs[3], char *line, t_import *imp)
{
	t_cart	norm;

	line = vrtxparser(line, imp->vt, imp->vn, &vrtxs[0]);
	line = vrtxparser(line, imp->vt, imp->vn, &vrtxs[1]);
	line = vrtxparser(line, imp->vt, imp->vn, &vrtxs[2]);
	normbuilder(getcart(imp->v, vrtxs[0].dot + 1),
		getcart(imp->v, vrtxs[1].dot + 1),
		getcart(imp->v, vrtxs[2].dot + 1),
		&norm);
	while (vectorlength(&norm) < 0.999 && notendedline(line))
	{
		vrtxs[1] = vrtxs[2];
		line = vrtxparser(line, imp->vt, imp->vn, &vrtxs[2]);
		normbuilder(getcart(imp->v, vrtxs[0].dot + 1),
			getcart(imp->v, vrtxs[1].dot + 1),
			getcart(imp->v, vrtxs[2].dot + 1),
			&norm);
	}
	return (line);
}

float	inverseuv(float uv)
{
	float	intgr;

	if (fabsf(uv) < 1)
	{
		uv += (uv < 0);
		return (uv);
	}
	if (uv > 0)
		uv = 1 - modff(uv, &intgr);
	else if (uv < 0)
		uv = 1 + modff(uv, &intgr);
	return (uv);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spheretxtrmapping.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:08:39 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/05 15:08:42 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	lngtdmapping(t_vrtx *vrtx, t_cart *yoz)
{
	t_cart	oy;
	t_cart	oz;
	float	scalar;
	int		sign;

	cartbuilder(0, 1, 0, &oy);
	cartbuilder(0, 0, 1, &oz);
	vectorsizing(1, yoz, yoz, NULL);
	ft_dotprod(yoz, &oz, &scalar);
	sign = 1;
	if (scalar < 0.001)
		sign = -1;
	ft_dotprod(yoz, &oy, &scalar);
	vrtx->uv.x = 0.5f + sign * 0.5f * acosf(scalar) / M_PI;
}

static void	spherepolymapping(t_vrtx *vrtx)
{
	t_cart	ox;
	t_cart	yoz;
	float	scalar;

	cartbuilder(1, 0, 0, &ox);
	ft_dotprod(&vrtx->srcnorm, &ox, &scalar);
	vrtx->uv.y = 0.5f - 0.5f * asinf(scalar) / M_PI_2;
	cartbuilder(0, vrtx->srcnorm.y, vrtx->srcnorm.z, &yoz);
	if (comparef(vectorlength(&yoz), 0, 0.001))
		vrtx->uv.x = 0.5f;
	else
		lngtdmapping(vrtx, &yoz);
}

static void	repairlgntd(t_vrtx vrtxs[3])
{
	int	j;
	int	k;

	j = -1;
	while (++j < 3)
	{
		if (vrtxs[j].dot < 2)
			vrtxs[j].uv.x = vrtxs[j + (!j) - (j == 2) - (j == 1)].uv.x;
		else if (comparef(vrtxs[j].uv.x, 0, 0.001))
		{
			k = -1;
			while (++k < 3)
			{
				if (vrtxs[k].dot > 1 && vrtxs[k].uv.x > 0.5f)
					vrtxs[j].uv.x = 1.f;
			}
		}
	}
}

void	spheremapping(t_poly *polys, int polynum)
{
	int	i;

	i = -1;
	while (++i < polynum)
	{
		spherepolymapping(&polys[i].vrtxs[0]);
		spherepolymapping(&polys[i].vrtxs[1]);
		spherepolymapping(&polys[i].vrtxs[2]);
		repairlgntd(polys[i].vrtxs);
	}
}

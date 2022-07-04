/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_checkerboard.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 21:56:41 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/04 20:26:51 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	my_mlx_get_pixel(t_data *data, t_cart *color)
{
	char			*dst;
	unsigned int	temp;
	long int		x;
	long int		y;

	x = lrintf(data->res.x * color->x);
	y = lrintf(data->res.y * color->y);
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	temp = *(unsigned int *)dst;
	(*color).x = get_r(temp) / 255.0f;
	(*color).y = get_g(temp) / 255.0f;
	(*color).z = get_b(temp) / 255.0f;
}

void	ft_checker_plane(t_cart *ox, t_cart *oy, t_cart *color, t_cart *phit)
{
	float	xhit;
	float	yhit;

	ft_dotprod(ox, phit, &xhit);
	ft_dotprod(oy, phit, &yhit);
	if (yhit < 0)
		yhit *= -1;
	if (xhit < 0)
		xhit *= -1;
	xhit = fmod(xhit / 100, 1) < 0.5;
	yhit = fmod(yhit / 100, 1) < 0.5;
	if ((xhit || yhit) && !(xhit * yhit))
	{
		color->x = 0.f;
		color->y = 0.f;
		color->z = 0.f;
	}
	else
	{
		color->x = 1.f;
		color->y = 1.f;
		color->z = 1.f;
	}
}

void	ft_texture_plane(t_crdstm *vects, t_cart *color,
			t_cart *phit, t_data *data)
{
	float			xhit;
	float			yhit;

	ft_dotprod(&vects->ox.vector, phit, &xhit);
	ft_dotprod(&vects->oy.vector, phit, &yhit);
	if (xhit < 0)
		xhit *= -1;
	if (yhit < 0)
		yhit *= -1;
	xhit = fmod(xhit / 100, 1);
	yhit = fmod(yhit / 100, 1);
	color->x = xhit;
	color->y = yhit;
	color->z = 0.f;
	my_mlx_get_pixel(data, color);
}

void	ft_checker_poly(t_cart *cuv)
{
	float	pattern[2];

	pattern[0] = fmod(cuv->x * 7, 1) < 0.5;
	pattern[1] = fmod(cuv->y * 7, 1) < 0.5;
	if ((pattern[0] || pattern[1]) && !(pattern[0] * pattern[1]))
	{
		cuv->x = 0.f;
		cuv->y = 0.f;
		cuv->z = 0.f;
	}
	else
	{
		cuv->x = 1.f;
		cuv->y = 1.f;
		cuv->z = 1.f;
	}
}

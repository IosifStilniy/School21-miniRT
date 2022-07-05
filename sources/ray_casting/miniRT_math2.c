/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/02 22:44:05 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	ft_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

void	ft_multvect(t_cart *vect, float multiplier, t_cart *dest)
{
	dest->x = vect->x * multiplier;
	dest->y = vect->y * multiplier;
	dest->z = vect->z * multiplier;
}

void	ft_multvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x * vect_b->x;
	dest->y = vect_a->y * vect_b->y;
	dest->z = vect_a->z * vect_b->z;
}

void	ft_invvect(t_cart *vect, t_cart *dest)
{
	dest->x = -vect->x;
	dest->y = -vect->y;
	dest->z = -vect->z;
}

void	ft_summvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x + vect_b->x;
	dest->y = vect_a->y + vect_b->y;
	dest->z = vect_a->z + vect_b->z;
}

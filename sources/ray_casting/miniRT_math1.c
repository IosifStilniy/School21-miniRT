/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/02 22:44:01 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_vectlen(t_cart *vect, float *dest)
{
	*dest = sqrtf(vect->x * vect->x + vect->y * vect->y + vect->z * vect->z);
}

void	ft_dotprod(t_cart *vect_a, t_cart *vect_b, float *dest)
{
	*dest = vect_a->x * vect_b->x + vect_a->y * vect_b->y
		+ vect_a->z * vect_b->z;
}

void	ft_subvects(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x - vect_b->x;
	dest->y = vect_a->y - vect_b->y;
	dest->z = vect_a->z - vect_b->z;
}

void	ft_crossprod(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->y * vect_b->z - vect_a->z * vect_b->y;
	dest->y = vect_a->z * vect_b->x - vect_a->x * vect_b->z;
	dest->z = vect_a->x * vect_b->y - vect_a->y * vect_b->x;
}

void	ft_vectnorm(t_cart *vector)
{
	float	length;

	ft_vectlen(vector, &length);
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

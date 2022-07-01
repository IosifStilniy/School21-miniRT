/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/07/01 19:58:23 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

float	ft_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

void	ft_multiply_vector(t_cart *vect, float multiplier, t_cart *dest)
{
	dest->x = vect->x * multiplier;
	dest->y = vect->y * multiplier;
	dest->z = vect->z * multiplier;
}

void	ft_multiply_vectors(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x * vect_b->x;
	dest->y = vect_a->y * vect_b->y;
	dest->z = vect_a->z * vect_b->z;
}

void	ft_inverse_vector(t_cart *vect, t_cart *dest)
{
	dest->x = -vect->x;
	dest->y = -vect->y;
	dest->z = -vect->z;
}

void	ft_summ_vectors(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x + vect_b->x;
	dest->y = vect_a->y + vect_b->y;
	dest->z = vect_a->z + vect_b->z;
}

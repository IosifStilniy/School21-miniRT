/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/26 20:44:12 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	ft_get_vector_length(t_cart *vect, float *dest)
{
	*dest = sqrtf(vect->x * vect->x + vect->y * vect->y + vect->z * vect->z);
}

void	ft_get_dot_product(t_cart *vect_a, t_cart *vect_b, float *dest)
{
	*dest = vect_a->x * vect_b->x + vect_a->y * vect_b->y + vect_a->z * vect_b->z;
}

void	ft_substract_vectors(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->x - vect_b->x;
	dest->y = vect_a->y - vect_b->y;
	dest->z = vect_a->z - vect_b->z;
}

void	ft_get_cross_product(t_cart *vect_a, t_cart *vect_b, t_cart *dest)
{
	dest->x = vect_a->y * vect_b->z - vect_a->z * vect_b->y;
	dest->y = vect_a->z * vect_b->x - vect_a->x * vect_b->z;
	dest->z = vect_a->x * vect_b->y - vect_a->y * vect_b->x;
}

void	ft_get_vector_norm(t_cart *vector)
{
	float	length;

	ft_get_vector_length(vector, &length);
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

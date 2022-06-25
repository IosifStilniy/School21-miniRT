/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/25 15:41:04 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

float	ft_get_vector_length(t_cart *vect)
{
	return (sqrt(vect->x * vect->x + vect->y * vect->y + vect->z * vect->z));
}

float	ft_get_dot_product(t_cart *vect_a, t_cart *vect_b)
{
	return (vect_a->x * vect_b->x + vect_a->y * vect_b->y + vect_a->z * vect_b->z);
}

t_cart	ft_substract_vectors(t_cart *vect_a, t_cart *vect_b)
{
	t_cart	vect_c;

	vect_c.x = vect_a->x - vect_b->x;
	vect_c.y = vect_a->y - vect_b->y;
	vect_c.z = vect_a->z - vect_b->z;
	return (vect_c);
}

t_cart	ft_get_cross_product(t_cart *vect_a, t_cart *vect_b)
{
	t_cart	vect_c;

	vect_c.x = vect_a->y * vect_b->z - vect_a->z * vect_b->y;
	vect_c.y = vect_a->z * vect_b->x - vect_a->x * vect_b->z;
	vect_c.z = vect_a->x * vect_b->y - vect_a->y * vect_b->x;
	return (vect_c);
}

t_cart	ft_get_vector_norm(t_cart *vector, float length)
{
	t_cart	norm_vector;

	norm_vector.x = vector->x / length;
	norm_vector.y = vector->y / length;
	norm_vector.z = vector->z / length;
	return (norm_vector);
}

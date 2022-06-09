/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_math2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 13:26:35 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/09 21:44:06 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

float	ft_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

t_cart	ft_multiply_vector(t_cart vect, float multiplier)
{
	t_cart	new_vect;

	new_vect.x = vect.x * multiplier;
	new_vect.y = vect.y * multiplier;
	new_vect.z = vect.z * multiplier;
	return (new_vect);
}

t_cart	ft_multiply_vectors(t_cart vect_a, t_cart vect_b)
{
	t_cart	new_vect;

	new_vect.x = vect_a.x * vect_b.x;
	new_vect.y = vect_a.y * vect_b.y;
	new_vect.z = vect_a.z * vect_b.z;
	return (new_vect);
}

t_cart	ft_inverse_vector(t_cart vect)
{
	t_cart	new_vect;

	new_vect.x = -vect.x;
	new_vect.y = -vect.y;
	new_vect.z = -vect.z;
	return (new_vect);
}

t_cart	ft_summ_vectors(t_cart vect_a, t_cart vect_b)
{
	t_cart	vect_c;

	vect_c.x = vect_a.x + vect_b.x;
	vect_c.y = vect_a.y + vect_b.y;
	vect_c.z = vect_a.z + vect_b.z;
	return (vect_c);
}

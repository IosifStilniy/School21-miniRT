/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:20:57 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/28 20:44:29 by dcelsa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	ft_atof(const char *num)
{
	float	result;
	int		sign;
	int		divider;

	result = 0;
	sign = 1;
	while (ft_strchr(SPACES, *num))
		num++;
	if ((*num == '-' || *num == '+') && *num++ == '-')
			sign = -1;
	while ('0' <= *num && *num <= '9')
		result = result * 10 + *num - '0';
	if (*num != '.')
		return (sign * result);
	divider = 10;
	num += 2;
	while ('0' <= *num && *num <= '9')
	{
		result += ((float)(*num - '0')) / divider;
		divider *= 10;
	}
	return (sign * result);
}

char	*ft_get_color_values(char *str, t_colrs *color, char *prog)
{
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("0123456789", *str))
		customerr(prog, INVDEF, TRUE);
	color->r = ft_atoi(str);
	if (!(0 <= color->r && color->r <= 255))
		customerr(prog, INVDEF, TRUE);
	while (ft_strchr(NUMSPACES, *str))
		str++;
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	color->g = ft_atoi(str);
	if (!(0 <= color->r && color->r <= 255))
		customerr(prog, INVDEF, TRUE);
	while (ft_strchr(NUMSPACES, *str))
		str++;
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	color->b = ft_atoi(str);
	while (ft_strchr("0123456789", *str))
		str++;
	return (str);
}

char	*ft_get_position_values(char *prog, char *str, t_cart *pos)
{
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("0123456789.", *str))
		customerr(prog, INVDEF, TRUE);
	pos->x = ft_atof(str);
	while (ft_strchr(NUMSPACES, *str))
		str++;
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	pos->y = ft_atof(str);
	while (ft_strchr(NUMSPACES, *str))
		str++;
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	pos->z = ft_atof(str);
	while (ft_strchr("0123456789.", *str))
		str++;
	return (str);
}

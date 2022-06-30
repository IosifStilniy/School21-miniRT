/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcelsa <dcelsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:20:57 by ncarob            #+#    #+#             */
/*   Updated: 2022/06/30 21:23:10 by dcelsa           ###   ########.fr       */
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
		result = result * 10 + *num++ - '0';
	if (*num++ != '.')
		return (sign * result);
	divider = 10;
	while ('0' <= *num && *num <= '9')
	{
		result += ((float)(*num++ - '0')) / divider;
		divider *= 10;
	}
	return (sign * result);
}

char	*ft_get_color_values(char *str, t_cart *color, char *prog)
{
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("0123456789", *str))
		customerr(prog, INVDEF, TRUE);
	color->x = ft_atoi(str) / 255.0f;
	if (!(0 <= color->x && color->x <= 1))
		customerr(prog, INVDEF, TRUE);
	str = skipnumnspaces(str, FALSE);
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	color->y = ft_atoi(str) / 255.0f;
	if (!(0 <= color->y && color->y <= 1))
		customerr(prog, INVDEF, TRUE);
	str = skipnumnspaces(str, FALSE);
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	color->z = ft_atoi(str) / 255.0f;
	if (!(0 <= color->z && color->z <= 1))
		customerr(prog, INVDEF, TRUE);
	str = skipnumnspaces(str, FALSE);
	return (str);
}

char	*ft_get_position_values(char *prog, char *str, t_cart *pos)
{
	while (ft_strchr(SPACES, *str))
		str++;
	if (!ft_strchr("-+0123456789.", *str))
		customerr(prog, INVDEF, TRUE);
	pos->x = ft_atof(str);
	str = skipnumnspaces(str, FALSE);
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	pos->y = ft_atof(str);
	str = skipnumnspaces(str, FALSE);
	if (*str++ != ',')
		customerr(prog, INVCRD, TRUE);
	pos->z = ft_atof(str);
	str = skipnumnspaces(str, FALSE);
	return (str);
}

char	*skipnumnspaces(char *str, t_bool onlyspaces)
{
	while (ft_strchr(SPACES, *str))
		str++;
	if (onlyspaces)
		return (str);
	while (ft_strchr("+-0123456789.", *str))
		str++;
	while (ft_strchr(SPACES, *str))
		str++;
	return (str);
}

int	primitivedefiner(char *str)
{
	int	i;

	i = -1;
	while (++i < NUMPRMTVS)
		if (!ft_strncmp(&PRMTVS[2 * i], str, 2))
			break ;
	return (i);
}

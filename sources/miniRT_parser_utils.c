/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/03 17:20:57 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/03 21:11:05 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

double	ft_atoidb(const char *num)
{
	double	result;
	int		sign;
	int		i;

	result = 0.0;
	sign = 1;
	i = 0;
	while ((num[i] > 8 && num[i] < 14) || num[i] == 32)
		i++;
	if (num[i] == 43 || num[i] == 45)
	{
		if (num[i] == 45)
			sign = -1;
		i++;
	}
	while (num[i] > 47 && num[i] < 58)
		result = result * 10 + num[i++] - 48;
	if (num[i] && num[i] == '.')
		i++;
	while (num[i] > 47 && num[i] < 58)
		result += (num[i++] - 48) / 10.0;
	return (result * sign);
}

int	ft_clear_char_array(char **array)
{
	int	i;

	if (!array)
		return (1);
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	return (1);
}

int	ft_get_position_values(char *str, t_cords *position)
{
	char	**nums;
	int		i;

	nums = ft_split(str, ',');
	if (!nums)
		return (1);
	i = -1;
	while (nums[++i])
	{
		if (!i)
			(*position).x = ft_atoidb(nums[i]);
		else if (i == 1)
			(*position).y = ft_atoidb(nums[i]);
		else if (i == 2)
			(*position).z = ft_atoidb(nums[i]);
		else
			return (ft_clear_char_array(nums));
	}
	ft_clear_char_array(nums);
	return (0);
}

int	ft_get_vector_values(char *str, t_cords *vector)
{
	char	**nums;
	int		i;

	nums = ft_split(str, ',');
	if (!nums)
		return (1);
	i = -1;
	while (nums[++i])
	{
		if (!i)
			(*vector).x = ft_atoidb(nums[i]);
		else if (i == 1)
			(*vector).y = ft_atoidb(nums[i]);
		else if (i == 2)
			(*vector).z = ft_atoidb(nums[i]);
		else
			return (ft_clear_char_array(nums));
	}
	ft_clear_char_array(nums);
	if ((*vector).x > 1.0 || (*vector).y > 1.0 || (*vector).z > 1.0
		|| (*vector).x < -1.0 || (*vector).y < -1.0 || (*vector).z < -1.0)
		return (1);
	return (0);
}

int	ft_get_color_values(char *str, t_colrs *color)
{
	char	**nums;
	int		i;

	nums = ft_split(str, ',');
	if (!nums)
		return (1);
	i = -1;
	while (nums[++i])
	{
		if (!i)
			(*color).r = ft_atoi(nums[i]);
		else if (i == 1)
			(*color).g = ft_atoi(nums[i]);
		else if (i == 2)
			(*color).b = ft_atoi(nums[i]);
		else
			return (ft_clear_char_array(nums));
	}
	ft_clear_char_array(nums);
	if ((*color).r < 0 || (*color).g < 0 || (*color).b < 0
		|| (*color).r > 255 || (*color).g > 255
		|| (*color).b > 255)
		return (1);
	return (0);
}

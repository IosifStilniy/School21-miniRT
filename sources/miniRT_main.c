/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/06 18:28:49 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc != 2)
		return (printf("The program takes in one argument\n"));
	info = ft_validate_file(argv[1]);
	if (!info)
		return (printf("The arguments id invalid\n"));
	info->mlx_ptr = mlx_init();
	info->wnd_ptr = mlx_new_window(info->mlx_ptr, 1024, 768, "MiniRT");
	info->data.img = mlx_new_image(info->mlx_ptr, 1024, 768);
	info->data.addr = mlx_get_data_addr(info->data.img,
			&info->data.bits_per_pixel, &info->data.line_length,
			&info->data.endian);
	my_mlx_pixel_put(&info->data, 0, 0, 0x00FF0000);
	mlx_put_image_to_window(info->mlx_ptr, info->wnd_ptr, info->data.img, 0, 0);
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_PRESS, 0, &ft_key_hook, info);
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_EXIT, 0, &ft_exit, NULL);
	mlx_loop(info->mlx_ptr);
	return (0);
}

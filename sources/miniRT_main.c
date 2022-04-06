/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 13:34:09 by ncarob            #+#    #+#             */
/*   Updated: 2022/04/05 12:54:31 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_info	*ft_init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->object = NULL;
	info->a_light = NULL;
	info->light = NULL;
	info->camera = NULL;
	info->mlx_ptr = NULL;
	info->wnd_ptr = NULL;
	return (info);
}

int	ft_clear_info(t_info *info)
{
	t_obj	*curr;

	if (!info)
		return (1);
	if (info->a_light)
		free(info->a_light);
	if (info->camera)
		free(info->camera);
	if (info->light)
		free(info->light);
	if (info->mlx_ptr)
		free(info->mlx_ptr);
	if (info->wnd_ptr)
		free(info->wnd_ptr);
	curr = NULL;
	while (info->object)
	{
		curr = info->object->next;
		free(info->object->identifier);
		free(info->object);
		info->object = curr;
	}
	free(info);
	return (1);
}

t_info	*ft_validate_file(char *filename)
{
	t_info	*info;
	int		len;
	int		fd;

	fd = 0;
	len = ft_strlen(filename);
	if (len < 4 || ft_strrchr(filename, 't') != &filename[len - 1]
		|| ft_strrchr(filename, 'r') != &filename[len - 2]
		|| ft_strrchr(filename, '.') != &filename[len - 3])
		return (NULL);
	if (len < 5 && ft_strrchr(filename, '/') == &filename[len - 4])
		return (NULL);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	info = ft_init_info();
	if (info && ft_read_information(fd, info))
	{
		close(fd);
		ft_clear_info(info);
		return (NULL);
	}
	close(fd);
	return (info);
}

int	ft_exit(void)
{
	ft_putendl_fd(ESC_TEXT, 1);
	exit(0);
}

int	ft_key_hook(int keycode, t_info *info)
{
	if (keycode == 53)
		ft_exit();
	(void)info;
	return (0);
}

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
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_PRESS, 0, &ft_key_hook, info);
	mlx_hook(info->wnd_ptr, X_EVENT_KEY_EXIT, 0, &ft_exit, NULL);
	mlx_clear_window(info->mlx_ptr, info->wnd_ptr);	
	mlx_loop(info->mlx_ptr);
	ft_clear_info(info);
	return (0);
}

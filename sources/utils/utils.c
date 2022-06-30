#include "minirt.h"

t_bool	comparef(float num, float ref, float interval)
{
	return (ref - interval < num && num < ref + interval);
}

int	file_check(char *file, char *prog)
{
	int	fd;

	if (!ft_strnstr(file + ft_strlen(file) - 3, ".rt", -1))
		customerr(prog, INVINP, FALSE);
	fd = 0;
	if (!access(file, F_OK) && !access(file, R_OK))
		fd = open(file, O_RDWR);
	if (!fd || (fd == -1 && errno == 21))
		error_handler(prog, file, -1);
	if (fd > 0 && !close(fd))
		fd = error_handler(prog, file, open(file, O_RDONLY));
	if (fd < 0)
		error_handler(prog, file, -1);
	return (fd);
}

t_obj	*objcast(t_list *lst)
{
	return (lst->content);
}

t_light	*lightcast(t_list *lst)
{
	return (lst->content);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

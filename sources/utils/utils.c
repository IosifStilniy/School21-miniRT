#include "minirt.h"

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

int	checknum(float num, char *str)
{
	int	count;

	count = 1 + (num < 0) + (num != truncf(num));
	while (num / 10 && count++)
		num /= 10;
	return (count);
}

t_obj	*objcast(t_list *lst)
{
	return (lst->content);
}

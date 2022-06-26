#include "minirt.h"

void	customerr(char *prog, char *txt, t_bool infile)
{
	if (infile)
		ft_putendl_fd("Error", 2);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(txt, 2);
	exit(1);
}

int	error_handler(char *prog, char *place, int funcres)
{
	if (funcres >= 0)
		return (funcres);
	ft_putstr_fd(prog, 2);
	ft_putstr_fd(": ", 2);
	if (place)
		ft_putstr_fd(place, 2);
	if (place)
		ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	exit(1);
}

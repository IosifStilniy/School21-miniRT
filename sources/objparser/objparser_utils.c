#include "minirt.h"

t_cart	*getcart(t_list *v, int indx)
{
	int	i;

	i = 0;
	while (++i < indx && v)
		v = v->next;
	return (v->content);
}

char	*notendedline(char *line)
{
	while (ft_strchr(SPACES, *line))
		++line;
	return (ft_strchr("123456789", *line));
}

char	*getfilename(char *start, char *end)
{
	char	*file;
	int		i;

	file = malloc(sizeof(*file) * (end - start + 1));
	i = -1;
	while (start + ++i < end)
		file[i] = *(start + i);
	file[i] = '\0';
	return (file);
}

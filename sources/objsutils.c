#include "minirt.h"

void	objexchanger(t_list *obj, t_list **dst, t_list **src)
{
	t_list	*prev;
	t_list	*crsr;

	crsr = *src;
	if (!crsr)
		return ;
	prev = NULL;
	while (crsr && crsr != obj)
	{
		prev = crsr;
		crsr = crsr->next;
	}
	if (!crsr)
		return ;
	if (prev)
		prev->next = crsr->next;
	else
		*src = crsr->next;
	ft_lstadd_front(dst, obj);
}

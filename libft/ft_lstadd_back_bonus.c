/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:34:50 by ncarob            #+#    #+#             */
/*   Updated: 2021/10/13 18:34:50 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*copy;

	copy = *lst;
	if (!*lst)
		ft_lstadd_front(lst, new);
	else
	{
		while (copy->next)
			copy = copy->next;
		copy->next = new;
	}
}

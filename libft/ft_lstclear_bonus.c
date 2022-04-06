/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarob <ncarob@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:34:55 by ncarob            #+#    #+#             */
/*   Updated: 2021/11/18 22:53:06 by ncarob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*curr;
	t_list	*next;

	if (!del)
		return ;
	curr = *lst;
	*lst = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->content)
			del(curr->content);
		free(curr);
		curr = next;
	}
}

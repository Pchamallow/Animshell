/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:14:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/23 11:30:54 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **head, t_list *newer)
{
	t_list	*last;

	if (!head || !newer)
		return ;
	if (!*head)
	{
		*head = newer;
		return ;
	}
	else
	{
		last = ft_lstlast(*head);
		last->next = newer;
	}
	return ;
}

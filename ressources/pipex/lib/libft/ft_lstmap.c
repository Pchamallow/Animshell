/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:14:41 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 19:34:24 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list;
	t_list	*temp;
	t_list	*tmp_obj;

	if (!lst || !f || !del)
		return (NULL);
	list = NULL;
	while (lst)
	{
		tmp_obj = f(lst->content);
		temp = ft_lstnew(tmp_obj);
		if (!temp)
		{
			del(tmp_obj);
			ft_lstclear(&list);
			return (NULL);
		}
		ft_lstadd_back(&list, temp);
		lst = lst->next;
	}
	return (list);
}

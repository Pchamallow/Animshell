/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:14:01 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/22 16:46:47 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_token_new(char *str, t_token_type token_type)
{
	t_token	*token;

	if (!str)
	{
		ft_printf_fd(2, "No value to extract\n");
		return (NULL);
	}
	token = malloc(sizeof (t_token));
	if (!token)
	{
		ft_printf_fd(2, "Error malloc when creating token\n");
		free(str);
		return (NULL);
	}
	ft_bzero(token, sizeof(t_token));
	token->type = token_type;
	token->value = str;
	token->next = NULL;
	return (token);
}

t_token	*ft_token_last(t_token *lst)
{
	if (!lst)
		return (lst);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_token_add_back(t_token **head, t_token *newer, char *line)
{
	t_token	*last;

	if (!head || !newer)
	{
		if (head)
			ft_token_lstclear(head);
		free(line);
		exit(2);
	}
	if (!*head)
	{
		*head = newer;
		return ;
	}
	else
	{
		last = ft_token_last(*head);
		last->next = newer;
	}
	return ;
}

void	ft_token_delone(t_token *lst, void (*del)(void *))
{
	// printf(" ! DELETE ! \n");
	if (!lst || !del)
		return ;
	// printf("value = %s\n", lst->value);
	// printf("fd = %d\n", lst->fd);
	del(lst->value);
	if (lst->cmd_path)
		free(lst->cmd_path);
	if (lst->path_explicite)
		free(lst->path_explicite);
	if (lst->cmd_args)
		free_strv(lst->cmd_args);
	if (lst->args_execve)
		free_strv(lst->args_execve);
	// if (lst->fd) // si >= 0 exit tout 
	if (lst->fd && lst->fd >= 0) // si >= 0 exit tout 
		close(lst->fd);
	free(lst);
	return ;
}

void	ft_token_lstclear(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head)
		return ;
	current = *head;
	// printf("-----------------DELETE\n");
	while (current)
	{
		// printf("FREE = %s\n", current->value);
		next = current->next;
		ft_token_delone(current, free);
		current = next;
	}
	*head = NULL;
}

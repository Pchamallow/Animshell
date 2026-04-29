/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:35:42 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/28 17:26:41 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strv(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_builtin(t_minishell *minishell)
{
	if (minishell->builtin.echo.result)
		free(minishell->builtin.echo.result);
	// minishell->builtin.echo.for_prompt = false;
}

void	free_all(t_minishell *minishell)
{
	free_heredoc(minishell);
	if (minishell->exec.envp)
		free_strv(minishell->exec.envp);
	if (minishell->exec.paths_for_search_cmd)
		free_strv(minishell->exec.paths_for_search_cmd);
	free_builtin(minishell);
	if (minishell->token)
		ft_token_lstclear(minishell->exec.first_token);
	if (minishell->exec.pipe_lst)
		lst_pipe_clear(&minishell->exec.pipe_lst);
	free(minishell->prompt);
	// print_pauline(minishell);// print la commande et les arguments
}

void	lst_pipe_clear(t_pipe **head)
{
	t_pipe	*current;
	t_pipe	*next;

	if (!head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}

void	free_heredoc(t_minishell *minishell)
{
	// free(minishell->here_doc->path_explicite);
	if (minishell->here_doc->value)
		free(minishell->here_doc->value);
	free(minishell->here_doc);
}

void	free_envp(t_minishell *minishell)
{
	free_strv(minishell->exec.envp);
	free_strv(minishell->exec.paths_for_search_cmd);
}

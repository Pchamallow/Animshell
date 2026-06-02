/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:47:49 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 14:21:38 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_first_pipe(t_token *token)
{
	t_token	*tmp;
	int		pipe;
	int		i;

	i = 0;
	pipe = 0;
	tmp = token;
	while (tmp != NULL && tmp->type != PIPE)
	{
		tmp = tmp->next;
		i++;
	}
	if (tmp != NULL && tmp->type == PIPE)
		pipe = 1;
	if (pipe)
		return (i);
	return (0);
}

static void	init_heredoc(t_minishell *minishell)
{
	minishell->here_doc = malloc(sizeof(t_token));
	if (!minishell->here_doc)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	minishell->here_doc->value = NULL;
	minishell->here_doc->fd = -1;
}

/* 
init_exec
 - find nb of pipes, 
 - fill 0 or NULL,
*/
void	init_exec(t_minishell *minishell)
{
	t_token	*tmp;
	int		first_pipe;

	first_pipe = find_first_pipe(minishell->token);
	if (first_pipe)
		minishell->exec.index_pipe = first_pipe;
	else
		minishell->exec.index_pipe = lst_size(minishell->token);
	minishell->exec.index_prev_pipe = 0;
	minishell->exec.last_pipe = minishell->token;
	minishell->exec.error_last_prompt = 0;
	minishell->exec.pipe_lst = NULL;
	tmp = minishell->token;
	while (tmp != NULL)
	{
		tmp->args_execve = NULL;
		tmp->cmd_path = NULL;
		tmp->cmd_args = NULL;
		tmp->file_input = 0;
		tmp->file_output = 0;
		tmp = tmp->next;
	}
	minishell->builtin.echo.result = NULL;
	minishell->builtin.cd.result = NULL;
	init_heredoc(minishell);
}

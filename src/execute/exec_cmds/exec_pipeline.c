/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 09:59:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 10:06:47 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_garbage(t_minishell *minishell, t_pipe *current)
{
	if (minishell->prompt)
	{
		free(minishell->prompt);
		minishell->prompt = NULL;
	}
	if (minishell->here_doc->fd != -1)
	{
		close_fd(&minishell->here_doc->fd);
		minishell->here_doc->fd = -1;
	}
	close_fds_pipe(current);
}

int	build_pipeline_structure(t_minishell *minishell, t_pipe *current,
	int *pipefd)
{
	minishell->exec.error_old = minishell->exec.error;
	minishell->exec.error = 0;
	if (read_tokens(minishell, current, minishell->exec.input_fd) != -1)
	{
		if (current->cmd && current->builtin_kind == NONE)
			init_args_execve(minishell, current);
	}
	else
		current->error = 1;
	if (minishell->exec.error == 130)
	{
		close_fd(&minishell->exec.input_fd);
		if (current->next || minishell->exec.nb_pipes)
		{
			minishell->exec.input_fd = pipefd[0];
			pipefd[0] = -1;
		}
		free_garbage(minishell, current);
		return (1);
	}
	return (0);
}

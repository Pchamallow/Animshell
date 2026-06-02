/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 12:07:53 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_parent_pipfd(t_minishell *minishell, t_pipe *current,
	int *pipefd)
{
	if (current->next || minishell->exec.nb_pipes)
	{
		minishell->exec.input_fd = pipefd[0];
		pipefd[0] = -1;
		close_fd(&pipefd[1]);
	}
}

static void	free_parent(t_minishell *minishell, t_pipe *current, int *pipefd)
{
	close_fd(&minishell->exec.input_fd);
	free_parent_pipfd(minishell, current, pipefd);
	free_garbage(minishell, current);
}

static void	exec_builtins(t_minishell *minishell, t_pipe *current)
{
	if (current->builtin_kind == CD)
		cd(minishell, current);
	if (current->builtin_kind == EXPORT)
		ft_export(minishell, current);
	if (current->builtin_kind == UNSET && !minishell->exec.nb_pipes)
		unset(minishell, current);
	if (current->builtin_kind == PWD)
		pwd_update(minishell);
	is_exit(minishell, current);
}

static void	exec_loop(t_minishell *minishell, t_pipe *current,
	int *pipefd, int *pid)
{
	while (current)
	{
		if (build_pipeline_structure(minishell, current, pipefd))
			break ;
		if (current->next)
		{
			if (pipe(pipefd) == -1)
				return ;
		}
		exec_builtins(minishell, current);
		*pid = fork();
		if (minishell->exec.pipe_actual == minishell->exec.nb_pipes)
			minishell->exec.last_pid = *pid;
		if (*pid == 0)
			exec_child(minishell, current, pipefd);
		free_parent(minishell, current, pipefd);
		minishell->exec.pipe_actual++;
		current = current->next;
	}
	get_exit_status(minishell, minishell->exec.last_pid);
}

/*
if current + is_next_pipe 
but current->output == IS_FILE
== ERROR 
-> next pipe will receive nothing,
so we close pipefd[1] == writing
*/
void	exec_cmds_pipe(t_minishell *minishell)
{
	t_pipe	*current;
	pid_t	pid;
	int		pipefd[2];

	ignore_signal();
	minishell->exec.error_sig = 0;
	current = minishell->exec.pipe_lst;
	minishell->exec.input_fd = -1;
	exec_loop(minishell, current, pipefd, &pid);
	if (minishell->exec.error_sig)
		minishell->exec.error = minishell->exec.error_sig;
}

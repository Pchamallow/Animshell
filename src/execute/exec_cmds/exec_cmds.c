/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/25 18:27:23 by pswirgie         ###   ########.fr       */
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

static void	free_garbage(t_minishell *minishell, t_pipe *current)
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

static int	build_pipeline_structure(t_minishell *minishell, t_pipe *current, int *pipefd)
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

static void	free_parent(t_minishell *minishell, t_pipe *current, int *pipefd)
{
	close_fd(&minishell->exec.input_fd);
	free_parent_pipfd(minishell, current, pipefd);
	free_garbage(minishell, current);
}

static void	exec_builtins(t_minishell *minishell, t_pipe *current)
{
	if (current->builtin_kind == IS_ECHO)
		echo(minishell, current);
	if (current->builtin_kind == CD)
		cd(minishell, current);
	if (current->builtin_kind == EXPORT)
		export(minishell, current);
	if (current->builtin_kind == UNSET && !minishell->exec.nb_pipes)
		unset(minishell, current);
	if (current->builtin_kind == PWD)
		pwd_update(minishell);
	is_exit(minishell, current);
}

static void exec_loop(t_minishell *minishell, t_pipe *current, int *pipefd, int *pid)
{
		while (current)
	{
		if (build_pipeline_structure(minishell, current, pipefd))
			break;
		if (current->next)
		{
			if (pipe(pipefd) ==  -1)
				return ;
		}
		exec_builtins(minishell, current);
		*pid = fork();
		if (minishell->exec.pipe_actual == minishell->exec.nb_pipes)
			minishell->exec.pipe_actual = pid;
		if (pid == 0)
			exec_child(minishell, current, pipefd);
		free_parent(minishell, current, pipefd);
		minishell->exec.pipe_actual++;
		current = current->next;
	}
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
	current = minishell->exec.pipe_lst;
	exec_loop(minishell, current, pipefd, &pid);
	get_exit_status(minishell, minishell->exec.last_pid);
}

/*
Print for tests
	char *str[] = {"ERROR", "TERMINAL", "IS_FILE", "IS_HEREDOC", "IS_PIPE"};
		printf("CHILD ----------------------------------\n");
		if (current->cmd)
			printf("current->value = %s\n", current->cmd->value);
		printf("current->input = %s\n", str[current->input]);
		printf("current->output = %s\n", str[current->output]);
		printf("minishell->exec.input_fd = %d\n", minishell->exec.input_fd);
		printf("pipefd[1] open = %d\n", pipefd[1]);
		printf("minishell->exec.input_fd closed = %d\n", minishell->exec.input_fd);
		printf("PARENT -----------------------------------\n");
		printf("minishell->exec.input_fd = %d\n", minishell->exec.input_fd);
		printf("pipefd[0] open = %d\n", pipefd[0]);
		printf("pipefd[1] open = %d\n", pipefd[1]);
		printf("at_least_one_pipe %d, %p\n", at_least_one_pipe, current->next);
		printf("pipefd[0] closed = %d\n", pipefd[0]);
		printf("pipefd[1] closed = %d\n", pipefd[1]);
*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:22:09 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 16:01:53 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_input(t_minishell *minishell, t_pipe *current)
{
	if (current->input == IS_FILE)
	{
		if (dup2(current->infile->fd, STDIN_FILENO) == -1)
			strerror_free_structure(minishell, "dup2 infile", 2);
		close_fd(&current->infile->fd);
	}
	else if (current->input == IS_PIPE)
	{
		if (dup2(minishell->exec.input_fd, STDIN_FILENO) == -1)
			strerror_free_structure(minishell, "dup2 pipefd[0]", 2);
		close_fd(&minishell->exec.input_fd);
	}
	else if (current->input == IS_HEREDOC)
	{
		if (dup2(minishell->here_doc->fd, STDIN_FILENO) == -1)
			strerror_free_structure(minishell, "dup2 here doc", 2);
		close_fd(&minishell->here_doc->fd);
	}
}

static void	redir_output(t_minishell *minishell, t_pipe *current, int *pipefd)
{
	if (current->output == IS_FILE)
	{
		if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
			strerror_free_structure(minishell, "dup2 outfile", 2);
		close_fd(&current->outfile->fd);
	}
	else if (current->output == IS_PIPE)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			strerror_free_structure(minishell, "dup2 pipefd[1]", 2);
		close_fd(&pipefd[1]);
	}
}

static void	exec_builtin_print(t_minishell *minishell, t_pipe *current,
	t_builtin_kind kind)
{
	if (kind == IS_ECHO)
		echo_print(minishell, current);
	else if (kind == PWD)
		pwd_print(minishell);
	else if (kind == EXPORT)
		export_print(minishell, current);
	else if (kind == ENV)
		env(minishell, current);
	else if (kind == EXPORT)
		export_print(minishell, current);
}

void	exec_child(t_minishell *minishell, t_pipe *current, int *pipefd)
{
	reset_signal_to_default();
	redir_input(minishell, current);
	close_fd(&minishell->exec.input_fd);
	redir_output(minishell, current, pipefd);
	if (current->next || minishell->exec.nb_pipes)
	{
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
	}
	close_fds_pipe(current);
	if (current->is_cmd && !current->error
		&& current->builtin_kind == NONE)
	{
		if (execve(current->cmd->cmd_path, current->cmd->args_execve,
				minishell->exec.envp) == -1)
			minishell->exec.error = errno;
		perror("minishell: execve: ");
	}
	else if (current->builtin_kind != NONE)
		exec_builtin_print(minishell, current, current->builtin_kind);
	free_all(minishell);
	exit(minishell->exec.error);
}

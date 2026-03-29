/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/27 17:08:48 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	close_all(t_parse *parse, int *pipefd)
{
	close_fds(parse);
	close(pipefd[0]);
	close(pipefd[1]);
}

static void	wrong_pid(t_parse *parse, int *pipefd)
{
	close_all(parse, pipefd);
	strerror_free_structure(parse, "fork", 2);
}

static void	exec_child(t_parse *parse, int *pipefd, char **envp)
{
	int	error;

	error = 0;
	if (dup2(parse->in.fd, STDIN_FILENO) == -1)
		strerror_free_structure(parse, "dup2", 2);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		strerror_free_structure(parse, "dup2", 2);
	close_all(parse, pipefd);
	error = execve(parse->in.cmd_path, parse->in.args_execve, envp);
	if (error == -1)
		strerror_free_structure(parse, parse->in.cmd, 127);
}

static void	exec_parent(t_parse *parse, int *pipefd, char **envp)
{
	int	error;

	error = 0;
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		strerror_free_structure(parse, "dup2", 2);
	if (dup2(parse->out.fd, STDOUT_FILENO) == -1)
		strerror_free_structure(parse, "dup2", 2);
	close_all(parse, pipefd);
	error = execve(parse->out.cmd_path, parse->out.args_execve, envp);
	if (error == -1)
		strerror_free_structure(parse, parse->out.cmd, 127);
}

void	exec_cmds(t_parse *parse, char **envp)
{
	int	pid;
	int	pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid == -1)
		wrong_pid(parse, pipefd);
	if (pid == 0)
	{
		if (parse->in.is_file == 1 && parse->in.is_cmd == 1)
			exec_child(parse, pipefd, envp);
		else
			close_all(parse, pipefd);
	}
	else
	{
		if (parse->out.is_file == 1 && parse->out.is_cmd == 1)
			exec_parent(parse, pipefd, envp);
		else
			close_all(parse, pipefd);
	}
	close_fds(parse);
	waitpid(pid, NULL, 0);
}

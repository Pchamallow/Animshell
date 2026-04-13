/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/13 17:09:50 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	close_fds_pipefd(t_minishell *minishell, int *pipefd)
// {
// 	close_fds(minishell);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// }

// static void	wrong_pid(t_minishell *minishell, int *pipefd)
// {
// 	close_all(minishell, pipefd);
// 	strerror_free_structure(minishell, "fork", 2);
// }

// static void	exec_child(t_minishell *minishell, int *pipefd, char **envp)
// {
// 	int	error;

// 	error = 0;
// 	if (dup2(parse->in.fd, STDIN_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	close_fds_pipefd(minishell, pipefd);
// 	error = execve(parse->in.cmd_path, parse->in.args_execve, envp);
// 	if (error == -1)
// 		strerror_free_structure(minishell, parse->in.cmd, 127);
// }

// static void	exec_parent(t_minishell *minishell, int *pipefd, char **envp)
// {
// 	int	error;

// 	error = 0;
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	if (dup2(parse->out.fd, STDOUT_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	close_fds_pipefd(minishell, pipefd);
// 	error = execve(parse->out.cmd_path, parse->out.args_execve, envp);
// 	if (error == -1)
// 		strerror_free_structure(minishell, parse->out.cmd, 127);
// }


// void	exec_cmds_pipe(t_minishell *minishell, char **envp)
// {
// 	int	pid;
// 	int	pipefd[2];

// 	pipe(pipefd);
// 	pid = fork();
// 	if (pid == -1)
// 		wrong_pid(minishell, pipefd);
// 	if (pid == 0)
// 	{
// 		if (minishell->exec.pipe_a->is_cmd == 1)
// 			exec_child(minishell, pipefd, envp);
// 		else
// 			close_fds_pipefd(minishell, pipefd);
// 	}
// 	else
// 	{
// 		if (minishell->exec.pipe_b->is_cmd == 1)
// 			exec_parent(minishell, pipefd, envp);
// 		else
// 			close_fds_pipefd(minishell, pipefd);
// 	}
// 	close_fds(minishell);
// 	waitpid(pid, NULL, 0);
// }

/*
inputs : 
-1 : issue with infile = return
0 : no infile = actual folder

soource input : infile, pipe, here_doc
sources ouput : terminal, outfile, pipe
*/




// ft_printf_fd(2, "\n--------------EXEC CMD----------------------\n");
// ft_printf_fd(2, "cmd_path %s\n", line->cmd->cmd_path);
// ft_printf_fd(2, "input : %d\n", line->input);
// ft_printf_fd(2, "ouput : %d\n", line->output);
// ft_printf_fd(2, "cmd :\n", line->output);
// ft_printf_fd(2, "EXEC CMD = %s\n", line->cmd->cmd_path);
// ft_printf_fd(2, "EXEC CMD = %s\n", line->cmd->value);
// ajouter securite -1
// ft_printf_fd(2, "ICI\n");

void	exec_cmd_no_pipe(t_minishell *minishell, char **envp)
{
	t_pipe *line;
	pid_t	pid;

	pid = fork();
	line = minishell->exec.pipe_a;
	if (pid == 0)
	{
		if (line->input == TERMINAL && line->output == TERMINAL)
		{
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (line->input == TERMINAL && line->output == IS_FILE)
		{
			if (dup2(line->outfile->fd, STDOUT_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (line->input == IS_FILE && line->output == TERMINAL)
		{
			if (dup2(line->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (line->input == IS_FILE && line->output == IS_FILE)
		{
			if (dup2(line->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			if (dup2(line->outfile->fd, STDOUT_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		free_all(minishell);
		exit(1);
	}
	else
	{
		close_fds(minishell);
		waitpid(pid, NULL, 0);
	}
	ft_printf_fd(2, "--------------------------------------------\n");
}

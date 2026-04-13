/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/13 21:56:45 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	close_fds_pipefd(t_minishell *minishell, int *pipefd)
// {
// 	close_fds(minishell, minishell->exec.pipe_lst);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// }

// static void	wrong_pid(t_minishell *minishell, int *pipefd)
// {
// 	close_fds_pipefd(minishell, pipefd);
// 	strerror_free_structure(minishell, "fork", 2);
// }

// static void	exec_file_to_pipe(t_minishell *minishell, t_pipe *pipe,
// 	int *pipefd, char **envp)
// {
// 	if (dup2(pipe->infile->fd, STDIN_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	// close_fds(minishell, minishell->exec.pipe_lst);
// 	// close_fds_pipefd(minishell, pipefd);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	// ft_printf_fd(2, "pipe_lst %s\n", pipe_lst->cmd->value);
// 	// ft_printf_fd(2, "pipe_lst %s\n", pipe_lst->cmd->cmd_path);
// 	// ft_printf_fd(2, "pipe_lst %s\n", pipe_lst->cmd->args_execve[0]);
// 	// print_double(pipe_lst->cmd->args_execve);
// 	execve(pipe->cmd->cmd_path, pipe->cmd->args_execve, envp);
// 	perror("execve");
// 	exit(1);
// }

// static void	exec_pipe_to_file(t_minishell *minishell, t_pipe *pipe,
// 	int *pipefd, char **envp)
// {
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	if (dup2(pipe->outfile->fd, STDOUT_FILENO) == -1)
// 		strerror_free_structure(minishell, "dup2", 2);
// 	// close_fds(minishell, minishell->exec.pipe_lst);
// 	// close_fds_pipefd(minishell, pipefd);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	// ft_printf_fd(2, "pipe_b %s\n", pipe_b->cmd->value);
// 	// ft_printf_fd(2, "pipe_b %s\n", pipe_b->cmd->cmd_path);
// 	// ft_printf_fd(2, "pipe_b %s\n", pipe_b->cmd->args_execve[0]);
// 	// print_double(pipe_b->cmd->args_execve);
// 	execve(pipe->cmd->cmd_path, pipe->cmd->args_execve, envp);
// 	perror("execve");
// 	exit(1);
// }

/*
1. 
create 2 forks : 1st command + 2nd command = children
parent = minishell

*/
void	exec_cmds_pipe(t_minishell *minishell, char **envp)
{
	t_pipe	*pipe_current;
	pid_t	pid;
	int		pipefd[2];
	int		prev_pipe;

	pipe_current = minishell->exec.pipe_lst;
	prev_pipe = -1;

	while (pipe_current)
	{
		if (pipe_current->next)
			pipe(pipefd);

		pid = fork();
		if (pid == -1)
			perror("fork");

		if (pid == 0) // children
		{
			printf("CHILD\n");
			// STDIN
			if (pipe_current->infile)
				dup2(pipe_current->infile->fd, STDIN_FILENO);
			else if (prev_pipe != -1)
				dup2(prev_pipe, STDIN_FILENO);

			// STDOUT
			if (pipe_current->outfile)
				dup2(pipe_current->outfile->fd, STDOUT_FILENO);
			else if (pipe_current->next)
				dup2(pipefd[1], STDOUT_FILENO);

			if (pipe_current->infile)
				close(pipe_current->infile->fd);

			if (pipe_current->outfile)
			{
				ft_printf_fd(2, "close fd\n");
				close(pipe_current->outfile->fd);
			}

			if (prev_pipe != -1)
				close(prev_pipe);

			if (pipe_current->next)
			{
				close(pipefd[0]);
				close(pipefd[1]);
				// if (pipe_current->outfile)
				// 	close(pipe_current->outfile->fd);
			}

			execve(pipe_current->cmd->cmd_path,
				pipe_current->cmd->args_execve, envp);

			perror(pipe_current->cmd->value);
			exit(127);
		}

		// parent
		printf("PARENT\n");
		printf("%s\n", pipe_current->cmd->value);
	
		if (pipe_current->infile)
			close(pipe_current->infile->fd);

		if (pipe_current->outfile)
		{
			ft_printf_fd(2, "close fd\n");
			close(pipe_current->outfile->fd);
		}

		if (prev_pipe != -1)
			close(prev_pipe);

		if (pipe_current->next)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}

		pipe_current = pipe_current->next;

		// if (pipe_current->outfile)
		// {
		// 	ft_printf_fd(2, "close fd\n");
		// 	close(pipe_current->outfile->fd);
		// }
	}

	// if (pipe_current->outfile->fd)
	// 	close(pipe_current->outfile->fd);
		
	if (prev_pipe != -1)
		close(prev_pipe);

	while (wait(NULL) > 0)
		;
}

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
	line = minishell->exec.pipe_lst;
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
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (line->input == IS_FILE && line->output == TERMINAL)
		{
			if (dup2(line->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (line->input == IS_FILE && line->output == IS_FILE)
		{
			if (dup2(line->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			if (dup2(line->outfile->fd, STDOUT_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
			perror("execve");
		}
		free_all(minishell);
		exit(1);
	}
	else
	{
		close_fds(minishell, minishell->exec.pipe_lst);
		waitpid(pid, NULL, 0);
	}
	ft_printf_fd(2, "--------------------------------------------\n");
}

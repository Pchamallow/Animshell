/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/17 19:19:20 by pswirgie         ###   ########.fr       */
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
	t_pipe *line;
	// char *line;
	pid_t	pid;
	int		pipefd[2];
	int		already_output;
	int		input_fd;
	// int		nb_pipes;
	// int		input_pipe;
	// int		output_pipe;
	int		i;
	int		pipe_to_execute;
	
	i = 0;
	// nb_pipes = minishell->exec.nb_pipes;
	pipe_to_execute = minishell->exec.nb_pipes;

	line = minishell->exec.pipe_lst;
	// printf("cmd 1 = %s\n",  line->cmd->value);
	// printf("cmd 2 = %s\n",  line->next->cmd->value);
	// printf("cmd 3 = %s\n",  line->next->next->cmd->value);
	
	// printf("nb_pipes = %d\n", pipe_to_execute);
	// if (nb_pipes >= 1)
	// {
	// 	while (nb_pipes)
	// 	{
	// 		pipe(pipefd);
	// 		nb_pipes--;
	// 	}
	// }

	/*  avoir le nb de la pipe actuel */
	// minishell->exec.index_pipe;
	
	// pid = fork();
	// printf("ICI = %s\n", minishell->exec.pipe_lst->cmd->value);
	// line = "cat";
	// free(line->cmd->value);
	// line->cmd->value = "cat";
	
	// if (minishell->exec.pipe_lst->next)
	// 	printf("dernier output = %d\n", minishell->exec.pipe_lst->next->output);
	pipe(pipefd);
	input_fd = 0;
	while (i <= pipe_to_execute)
	{
		pid = fork();
		already_output = 0;

		// printf("infile fd== %d\n", line->infile->fd);
		// input_pipe = minishell->exec.index_pipe;/*  nb pipe - 1 -> le nb de la pipe d avant   */
		// printf("input pipe == %d\n", input_pipe);
		// output_pipe = minishell->exec.index_pipe + 1;/* nb pipe + 1 -> le nb de la pipe d apres   */
		// printf("output pipe == %d\n", output_pipe);
		if (pid == 0)
		{
			printf("line->input = %d\n", line->input);
			printf("line->output = %d\n", line->output);
			if (line->input != ERROR && line->output != ERROR)
			{
				/* INPUT               */
				if (line->input == IS_FILE && line->output == IS_FILE)
				{
					if (dup2(line->infile->fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					if (dup2(line->outfile->fd, STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					already_output = 1;
				}
				
				else if (line->input == IS_FILE)
				{
					if (dup2(line->infile->fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				
				else if (line->input == IS_PIPE)
				{
					printf("LINE IN PIPE\n");
					if (dup2(input_fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				

				/* OUTPUT                          */
				if (line->output == IS_FILE && already_output == 0)
				{
					if (dup2(line->outfile->fd, STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				// printf("already_ouput = %d\n", already_output);
				else if (line->output == IS_PIPE && already_output == 0)
				{
					printf("LINE OUT PIPE\n");
					if (dup2(pipefd[1], STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					// output_fd = 
				}
				// input_fd = pipefd[0];
				close(pipefd[0]);
				close(pipefd[1]);
				
				close_fds(minishell, minishell->exec.pipe_lst);
				execve(line->cmd->cmd_path, line->cmd->args_execve, envp);
				perror("execve");
				
				free_all(minishell);
				exit(1);
			}
		}
		i++;
		printf("i = %d\n", i);
		printf("pipe to execute = %d\n", pipe_to_execute);
		input_fd = pipefd[0];
		line = line->next;
		close_fds(minishell, minishell->exec.pipe_lst);
		close(pipefd[1]);
	}
	waitpid(pid, NULL, 0);
	close(input_fd);
	close(pipefd[0]);
	ft_printf_fd(2, "--------------------------------------------\n");
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/21 12:52:25 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmds_pipe(t_minishell *minishell, char **envp)
{
	t_pipe *current;
	pid_t	pid;
	int		pipefd[2];
	int		already_output;
	int		input_fd;
	int		i;
	int		pipe_to_execute;
	int		is_next_pipe;
	
	i = 0;
	// nb_pipes = minishell->exec.nb_pipes;
	pipe_to_execute = minishell->exec.nb_pipes;

	
	current = minishell->exec.pipe_lst;

	
	// if (minishell->exec.pipe_lst->next)
	// 	printf("dernier output = %d\n", minishell->exec.pipe_lst->next->output);
	// pipe(pipefd);
	input_fd = 0;
	
	while (current)
	{
		
		if (read_tokens(minishell, current, envp) != -1)
		{
			if (current->cmd)
				init_args_execve(minishell, current);
		}
		else
		{
			printf("WRONG CMD OR FILE\n");
			return;
		}
		
		if (current->next)
		{
			pipe(pipefd);
			is_next_pipe = 1;
		}
		else 
			is_next_pipe = 0;

		
		pid = fork();
		already_output = 0;

		// read_tokens(minishell, pipe, envp);
		// if (pipe->cmd)
		// 	init_args_execve(minishell, pipe);

		// printf("infile fd== %d\n", current->infile->fd);
		// input_pipe = minishell->exec.index_pipe;/*  nb pipe - 1 -> le nb de la pipe d avant   */
		// printf("input pipe == %d\n", input_pipe);
		// output_pipe = minishell->exec.index_pipe + 1;/* nb pipe + 1 -> le nb de la pipe d apres   */
		// printf("output pipe == %d\n", output_pipe);
		if (pid == 0)
		{
			printf("current->input = %d\n", current->input);
			printf("current->output = %d\n", current->output);
			if (current->input != ERROR && current->output != ERROR)
			{
				/* INPUT               */
				if (current->input == IS_FILE && current->output == IS_FILE)
				{
					if (dup2(current->infile->fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					already_output = 1;
				}
				
				else if (current->input == IS_FILE)
				{
					if (dup2(current->infile->fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				
				else if (current->input == IS_PIPE)
				{
					printf("current IN PIPE\n");
					if (dup2(input_fd, STDIN_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				

				/* OUTPUT                          */
				if (current->output == IS_FILE && already_output == 0)
				{
					if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
				}
				// printf("already_ouput = %d\n", already_output);
				else if (current->output == IS_PIPE && already_output == 0)
				{
					printf("current OUT PIPE\n");
					if (dup2(pipefd[1], STDOUT_FILENO) == -1)
						strerror_free_structure(minishell, "dup2", 2);
					// output_fd = 
				}
				// input_fd = pipefd[0];
				if (is_next_pipe)
				{
					close(pipefd[0]);
					close(pipefd[1]);
				}
				
				close_fds(minishell, current);
				execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
				perror("execve");
				
				free_all(minishell);
				exit(1);
			}
		}
		printf("i = %d\n", i);
		printf("pipe to execute = %d\n", pipe_to_execute);
		if (is_next_pipe)
		{
			input_fd = pipefd[0];
			close(pipefd[1]);
		}
		close_fds(minishell, current);
		current = current->next;
	}
	waitpid(pid, NULL, 0);
	// close(input_fd);
	// close(pipefd[0]);
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
// ft_printf_fd(2, "cmd_path %s\n", current->cmd->cmd_path);
// ft_printf_fd(2, "input : %d\n", current->input);
// ft_printf_fd(2, "ouput : %d\n", current->output);
// ft_printf_fd(2, "cmd :\n", current->output);
// ft_printf_fd(2, "EXEC CMD = %s\n", current->cmd->cmd_path);
// ft_printf_fd(2, "EXEC CMD = %s\n", current->cmd->value);
// ajouter securite -1
// ft_printf_fd(2, "ICI\n");

void	exec_cmd_no_pipe(t_minishell *minishell, char **envp)
{
	t_pipe *current;
	pid_t	pid;

	pid = fork();
	current = minishell->exec.pipe_lst;
	if (pid == 0)
	{
		if (current->input == TERMINAL && current->output == TERMINAL)
		{
			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (current->input == TERMINAL && current->output == IS_FILE)
		{
			if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (current->input == IS_FILE && current->output == TERMINAL)
		{
			if (dup2(current->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
			perror("execve");
		}
		
		else if (current->input == IS_FILE && current->output == IS_FILE)
		{
			if (dup2(current->infile->fd, STDIN_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
				strerror_free_structure(minishell, "dup2", 2);
			close_fds(minishell, minishell->exec.pipe_lst);
			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
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

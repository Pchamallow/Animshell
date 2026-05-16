/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/15 15:37:39 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_array_built_in(int(**array_built_in)(t_minishell *, t_pipe *))
{
	array_built_in[IS_ECHO] = echo;
	array_built_in[PWD] = pwd;
	array_built_in[EXPORT] = export_print;
	array_built_in[UNSET]= unset;
	array_built_in[ENV] = env;
	return ;
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
	t_pipe *current;
	pid_t	pid;
	int		pipefd[2];
	int		already_output;
	int		input_fd;
	int		is_next_pipe;
	int		at_least_one_pipe;
	int		(*array_built_in[8])(t_minishell *, t_pipe *);
	
	ignore_signal();
	current = minishell->exec.pipe_lst;
	init_array_built_in(array_built_in);
	input_fd = -1;
	at_least_one_pipe = 0;
	
	while (current)
	{
		minishell->exec.error = 0;
		
		if (read_tokens(minishell, current, input_fd) != -1)
		{
			if (current->cmd)
				init_args_execve(minishell, current);
		}
		else
			current->error = 1;
		if (minishell->exec.error == 130)
			break ;
		if (current->next)
		{
			pipe(pipefd);
			is_next_pipe = 1;
			at_least_one_pipe = 1;
		}
		else 
			is_next_pipe = 0;

		if (current->builtin_kind == IS_ECHO)
			echo_for_prompt(minishell, current);
		if (current->builtin_kind == CD)
			cd(minishell, current);
		if (current->builtin_kind == EXPORT)
			export(minishell, current);
		if (current->builtin_kind == UNSET && !at_least_one_pipe)
			unset(minishell, current);
		
		is_exit(minishell, current);
		
		pid = fork();
		already_output = 0;

		if (pid == 0)
		{
			reset_signal_to_default();


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
				if (dup2(input_fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				close_fd(&input_fd);
			}
			
			else if (current->input == IS_HEREDOC)
			{
				if (dup2(minishell->here_doc->fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				close_fd(&minishell->here_doc->fd);
			}
			
			close_fd(&input_fd);
			
			
			/* OUTPUT                          */
			if (current->output == IS_FILE && already_output == 0)
			{
				if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				close_fd(&current->outfile->fd);
			}
			else if (current->output == IS_PIPE
				&& already_output == 0)
			{
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				close_fd(&pipefd[1]);
			}

			if (current->next || at_least_one_pipe)
			{
				close_fd(&pipefd[0]);
				close_fd(&pipefd[1]);
			}
			
			close_fds_pipe(current);

			if (current->is_cmd && !current->error
				&& current->builtin_kind == NONE)
			{
				execve(current->cmd->cmd_path, current->cmd->args_execve, minishell->exec.envp);
				perror("execve");
			}
			else if (current->builtin_kind == IS_ECHO
				|| current->builtin_kind == PWD
				|| current->builtin_kind == ENV
				|| current->builtin_kind == EXPORT)
				array_built_in[current->builtin_kind](minishell, current);
			
			free_all(minishell);
			exit(minishell->exec.error);
			
		}
		close_fd(&input_fd);

		if (current->next || at_least_one_pipe)
		{
			input_fd = pipefd[0];
			pipefd[0] = -1;
			close_fd(&pipefd[1]);
		}
		
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
		current = current->next;
	}
	get_exit_status(minishell);
	ft_printf_fd(2, "--------------------------------------------\n");
}
/*
Print for tests
	char *str[] = {"ERROR", "TERMINAL", "IS_FILE", "IS_HEREDOC", "IS_PIPE"};
		printf("CHILD ----------------------------------\n");
		if (current->cmd)
			printf("current->value = %s\n", current->cmd->value);
		printf("current->input = %s\n", str[current->input]);
		printf("current->output = %s\n", str[current->output]);
		printf("input_fd = %d\n", input_fd);
		printf("pipefd[1] open = %d\n", pipefd[1]);
		printf("input_fd closed = %d\n", input_fd);
		printf("PARENT -----------------------------------\n");
		printf("input_fd = %d\n", input_fd);
		printf("pipefd[0] open = %d\n", pipefd[0]);
		printf("pipefd[1] open = %d\n", pipefd[1]);
		printf("at_least_one_pipe %d, %p\n", at_least_one_pipe, current->next);
		printf("pipefd[0] closed = %d\n", pipefd[0]);
		printf("pipefd[1] closed = %d\n", pipefd[1]);
		*/
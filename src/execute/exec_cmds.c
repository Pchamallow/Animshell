/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:01:28 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 18:14:42 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//faire un tableau de fonction

void	init_array_built_in(int(**array_built_in)(t_minishell *, t_pipe *))
{
	array_built_in[IS_ECHO] = echo;
	array_built_in[PWD] = pwd;
	 array_built_in[EXPORT]= export;
	// array_built_in[UNSET]= ;
	 array_built_in[ENV]= env;
	// array_built_in[EXIT]= ;
	return ;
}





// void	exec_built_in(t_minishell *minishell, t_pipe *pipe)
// {
// 	if (pipe->builtin_kind)
// }

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
	// int		not_write;
	
	ignore_signal();
	current = minishell->exec.pipe_lst;
	init_array_built_in(array_built_in);
	// if (minishell->exec.pipe_lst->next)
	// 	printf("dernier output = %d\n", minishell->exec.pipe_lst->next->output);
	// pipe(pipefd);
	input_fd = 0;
	at_least_one_pipe = 0;
	
	while (current)
	{
		// printf(ERROR_MSG("new command -----------------------\n"));
		if (read_tokens(minishell, current) != -1)
		{
			if (current->cmd)
				init_args_execve(minishell, current);
			// not_write = 0;
		}
		else
		{
			// printf("WRONG CMD OR FILE\n");
			current->error = 1;
			// not_write = 1;
		}

		// printf("current built-in = %d\n", current->builtin_kind);
		//print
		// if (current->cmd)
		// 	printf("exec command = %s\n", current->cmd->value);
		
		if (current->next)
		{
			// printf("create 1 pipe\n");
			pipe(pipefd);
			// minishell->exec.pipe_lst->pipfd1 = pipefd[1];
			is_next_pipe = 1;
			at_least_one_pipe = 1;
		}
		else 
			is_next_pipe = 0;

		if (current->builtin_kind == IS_ECHO)
			echo_for_prompt(minishell, current);
		if (current->builtin_kind == CD)
			cd(minishell, current);

		// if (current->output ==  && is_next_pipe)
		// 	current->error = 1;
		
		//--------------------------------------------------------------
		//Si pas de pipes && cmd == built-in et != IS_ECHO
		//-> pas de fork :
		// if (!at_least_one_pipe && current->builtin_kind
		// 		&&current->builtin_kind != IS_ECHO)
		// {
		// 	array_built_in[current->builtin_kind](minishell, current);
		// 	close_fds_pipe(current);//demander à Pauline si ça marche dans ce cas
		// 	return ;
		// }
		//--------------------------------------------------------------

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
			reset_signal_to_default();
			// printf("-------- child \n");
			printf("current->input = %d\n", current->input);
			printf("current->output = %d\n", current->output);

			/* INPUT               */
			if (current->input == IS_FILE && current->output == IS_FILE)
			{
				if (dup2(current->infile->fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				// close_fd(current->outfile->fd);
				already_output = 1;
			}
			
			else if (current->input == IS_FILE)
			{
				if (dup2(current->infile->fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				// if (current->infile->fd >= 0)
				// 	close(current->infile->fd);
			}
			
			else if (current->input == IS_PIPE)
			{
				// printf("current IN PIPE\n");
				// printf("input_fd = %d\n", input_fd);
				if (dup2(input_fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				// printf("pas de probleme fd \n");
				close_fd(input_fd);
			}

			else if (current->input == IS_HEREDOC)
			{
				// printf("here doc fd = %d\n", minishell->here_doc->fd);
				if (dup2(minishell->here_doc->fd, STDIN_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				// printf("close heredoc\n");
				close_fd(minishell->here_doc->fd);
			}

			/* OUTPUT                          */
			if (current->output == IS_FILE && already_output == 0)
			{
				if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
				close_fd(current->outfile->fd);
			}
			// printf("already_ouput = %d\n", already_output);
			else if (current->output == IS_PIPE
				&& already_output == 0)
			{
				// printf("current OUT PIPE\n");
				// printf("pipefd 1 = %d\n", pipefd[1]);
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
					strerror_free_structure(minishell, "dup2", 2);
			}
			if (is_next_pipe
				|| (current->input == ERROR && current->output == ERROR
				&& is_next_pipe))
			{
				close_fd(pipefd[0]);
				close_fd(pipefd[1]);
			}		

			close_fds_pipe(current);

			// printf("current = %s\n", current->cmd->value);
			if (current->is_cmd && !current->error
				&& current->builtin_kind == NONE)
			{
				execve(current->cmd->cmd_path, current->cmd->args_execve, minishell->exec.envp);
				perror("execve");
			}
			else if (current->builtin_kind != NONE
				&& current->builtin_kind != CD)
				array_built_in[current->builtin_kind](minishell, current);
				// exec_built_in(minishell, current);
			
			free_all(minishell);
			exit(1);
			
		}
		// printf("pipe to execute = %d\n", pipe_to_execute);
		// printf("-------- parent \n");
		if (is_next_pipe)
		{
			// else
			// {
				close_fd(input_fd);
				input_fd = pipefd[0];
			// }
			// printf("input_fd = %d\n", input_fd);
			close_fd(pipefd[1]);
			
		}
		else if (at_least_one_pipe)
			close_fd(pipefd[0]);
		
		if (minishell->prompt)
		{
			free(minishell->prompt);
			minishell->prompt = NULL;
		}
		
		if (minishell->here_doc->fd != -1)
		{
			close_fd(minishell->here_doc->fd);
			minishell->here_doc->fd = -1;
		}
		
		close_fds_pipe(current);
		// free_all(minishell);
		current = current->next;
	}
	while(wait(NULL) > 0);

	get_exit_status(minishell);

	// close_fds_pipe(current);
	
	ft_printf_fd(2, "--------------------------------------------\n");
}


// void	exec_cmds(t_minishell *minishell, char **envp)
// {
// 	t_pipe *current;

	
// 	while (current->next)
// 	{
// 		exec_cmds_pipe(minishell, envp);
// 		// but en cas de re
// 	}
// }



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

// void	exec_cmd_no_pipe(t_minishell *minishell, char **envp)
// {
// 	t_pipe *current;
// 	pid_t	pid;

// 	pid = fork();
// 	current = minishell->exec.pipe_lst;
// 	if (pid == 0)
// 	{
// 		if (current->input == TERMINAL && current->output == TERMINAL)
// 		{
// 			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
// 			perror("execve");
// 		}
		
// 		else if (current->input == TERMINAL && current->output == IS_FILE)
// 		{
// 			if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
// 				strerror_free_structure(minishell, "dup2", 2);
// 			close_fds_pipe(minishell, minishell->exec.pipe_lst);
// 			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
// 			perror("execve");
// 		}
		
// 		else if (current->input == IS_FILE && current->output == TERMINAL)
// 		{
// 			if (dup2(current->infile->fd, STDIN_FILENO) == -1)
// 				strerror_free_structure(minishell, "dup2", 2);
// 			close_fds_pipe(minishell, minishell->exec.pipe_lst);
// 			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
// 			perror("execve");
// 		}
		
// 		else if (current->input == IS_FILE && current->output == IS_FILE)
// 		{
// 			if (dup2(current->infile->fd, STDIN_FILENO) == -1)
// 				strerror_free_structure(minishell, "dup2", 2);
// 			if (dup2(current->outfile->fd, STDOUT_FILENO) == -1)
// 				strerror_free_structure(minishell, "dup2", 2);
// 			close_fds_pipe(minishell, minishell->exec.pipe_lst);
// 			execve(current->cmd->cmd_path, current->cmd->args_execve, envp);
// 			perror("execve");
// 		}
// 		free_all(minishell);
// 		exit(1);
// 	}
// 	else
// 	{
// 		close_fds_pipe(minishell, minishell->exec.pipe_lst);
// 		waitpid(pid, NULL, 0);
// 	}
// 	ft_printf_fd(2, "--------------------------------------------\n");
// }

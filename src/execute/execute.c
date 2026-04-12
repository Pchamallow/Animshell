/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/12 15:48:07 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pipe(t_pipe *pipe)
{
	if (pipe->infile->close == 0)
		close(pipe->infile->fd);
	if (pipe->outfile->close == 0)
		close(pipe->outfile->fd);
}

void handle_sigint(int sig)
{
    /*struct termios oldt, newt;*/
	/*t_token	*first_token;*/

	/*first_token = NULL;*/
	(void)sig;
	write(1, "\nminishell$ ", 12); // réaffiche le prompt
}

static void init_pipe(t_pipe **pipe)
{
    *pipe = ft_calloc(1, sizeof(t_pipe));
    (*pipe)->is_cmd = 0;
    (*pipe)->nb_args = 0;
	(*pipe)->built_in = NONE;
    (*pipe)->input = TERMINAL;
    (*pipe)->output = TERMINAL;
    (*pipe)->infile = NULL;
    (*pipe)->outfile = NULL;
}

/*
Put values 0 or NULL in order to reuse after
1 for input and output for terminal by default
*/
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;
	
	init_pipe(&minishell->exec.pipe_a);
	init_pipe(&minishell->exec.pipe_b);

	minishell->exec.input = 0;
	minishell->exec.output = 0;
	minishell->exec.index_pipe = 0;
	minishell->exec.last_pipe = minishell->token;
	tmp = minishell->token;
	while (tmp != NULL)
	{
		tmp->args_execve = NULL;
		tmp->cmd_path = NULL;
		tmp->cmd_args = NULL;
		tmp->file_input = 0;
		tmp->file_output = 0;
		tmp->close = 0;
		tmp = tmp->next;
	}
}

int execute(t_minishell *minishell, char **envp)
{
	/*             EXECUTION       V001      */
	// init_exec(minishell);
	// read_tokens(minishell, minishell->exec.pipe_a,
	// 	minishell->token, envp);
	// ft_printf_fd(2, "------------------\n");
	// // read_tokens(minishell, minishell->exec.pipe_b,
	// // 	minishell->token, envp);
	// init_args_execve(minishell, minishell->exec.pipe_a);
	// exec_cmd(minishell, envp);
	// print_pauline(minishell);//TO DELETE
	/************************************************/

	signal(SIGINT, handle_sigint);
	
	/*  BOUCLE WHILE  **********************************/
	//variables pour boucle while :
	char	*line;
	// bool	is_free;
	t_token	*first_token;

	first_token = NULL;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			rl_clear_history();
			exit (0);
		}

		if (*line)
			add_history(line);

		// PARSING ICI :************************
		if (separate_into_tokens(line, &first_token) == 1)
			continue ;
		parse_tokens(&first_token);
		// *************************************
		
		free(line);
		
		(void)envp;
		(void)minishell;
		// /* EXECUTION ICI ********************************/
		if (first_token)
		{
			minishell->token = first_token;
			minishell->exec.first_token = &first_token;
		}
		else
    		minishell->token = NULL;
		
		if (minishell->token)
		{
			init_exec(minishell);
			read_tokens(minishell, minishell->exec.pipe_a, envp);
			if (minishell->exec.pipe_a->is_cmd == 1)
			{
				init_args_execve(minishell, minishell->exec.pipe_a);
				exec_cmd(minishell, envp);
			}
		}
		// /************************************************/
		
		free_all(minishell);
		
		// read(STDIN_FILENO, &c, 1);
	}

	// revenir au terminal normal
	
	return (0);
}

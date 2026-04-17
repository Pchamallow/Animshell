/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/17 14:15:33 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    /*struct termios oldt, newt;*/
	/*t_token	*first_token;*/

	/*first_token = NULL;*/
	(void)sig;
	write(1, "\nminishell$ ", 12); // réaffiche le prompt
}

static void init_pipe(t_minishell *minishell)
{
	t_pipe	*current;
	int	max;
	int	i;

	i = 0;
	max = minishell->exec.nb_pipes;
	// printf("max = %d\n", max);
	minishell->exec.pipe_lst = ft_calloc(1, sizeof(t_pipe));
	if (!minishell->exec.pipe_lst)
		return ;
	current = minishell->exec.pipe_lst;
	while (i <= max)
	{
		// printf("ICI\n");
		current->is_cmd = 0;
		current->nb_args = 0;
		current->built_in = NONE;
		current->input = TERMINAL;
		current->output = TERMINAL;
		current->infile = NULL;
		current->outfile = NULL;
		if (i < max)
		{
			current->next = ft_calloc(1, sizeof(t_pipe));
			if (!current->next)
				return ;
			current = current->next;
		}
		i++;
	}
}


void	nb_pipes(t_minishell *minishell, t_token *first)
{
	t_token *token;
	int		pipes;

	pipes = 0;
	token = first;
	while (token)
	{
		if (token->type == PIPE)
			pipes++;
		token = token->next;
	}
	minishell->exec.nb_pipes = pipes;
}

/*
Put values 0 or NULL in order to reuse after
1 for input and output for terminal by default
*/
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;

	nb_pipes(minishell, minishell->token);
	// printf("ICI\n");
	init_pipe(minishell);
	// ft_printf_fd(2, "test = %d\n", minishell->exec.pipe_lst->is_cmd);

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
		tmp = tmp->next;
	}
}

void read_all_pipes(t_minishell *minishell, char **envp)
{
	t_pipe *pipe;
	int	max;
	int	i;

	i = 0;
	max = minishell->exec.nb_pipes;
	pipe = minishell->exec.pipe_lst;
	while (i <= max)
	{
		read_tokens(minishell, pipe, envp);
		if (pipe->cmd)
			init_args_execve(minishell, pipe);
		pipe = pipe->next;
		i++;
	}
}

int execute(t_minishell *minishell, char **envp)
{
	/*             EXECUTION       V001      */
	// init_exec(minishell);
	// read_tokens(minishell, minishell->exec.pipe_lst,
	// 	minishell->token, envp);
	// ft_printf_fd(2, "------------------\n");
	// // read_tokens(minishell, minishell->exec.pipe_b,
	// // 	minishell->token, envp);
	// init_args_execve(minishell, minishell->exec.pipe_lst);
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
	minishell->exec.error = 0;

	while (1)
	{
		// printf("MAIN LOOP PID: %d\n", getpid());
		line = readline("minishell$ ");
		if (!line)
		{
			// free_all(minishell);
			rl_clear_history();
			printf("exit\n");
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
		
		/* PARSING **********************************/
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
			read_all_pipes(minishell, envp);
			if (minishell->exec.pipe_lst->is_cmd == 1)
			{
				if (minishell->exec.nb_pipes > 0)
					exec_cmds_pipe(minishell, envp);
				else if (minishell->exec.pipe_lst->built_in != NONE)
					echo(minishell);
				else
					exec_cmds_pipe(minishell, envp);
					// exec_cmd_no_pipe(minishell, envp);
			}
		}
		/************************************************/
		
		free_all(minishell);
		
		// cat < infile.txt | wc -c > outfile.txt
		
	}
	return (0);
}

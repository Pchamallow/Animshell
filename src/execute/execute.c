/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/25 18:36:03 by pswirgie         ###   ########.fr       */
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


int	nb_pipes(t_token *first)
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
	return (pipes);
}

/* 
Find nb of pipes, 
fill 0 or NULL,
*/
static void init_pipe(t_minishell *minishell)
{
	t_pipe	*current;
	int	max;
	int	i;

	i = 0;
	max = nb_pipes(minishell->token);
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
		current->error = 0;
		current->builtin_kind = NONE;
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


// void init_paths_for_search_cmd(t_minishell *minishell, char **envp)
// {
// 	char *paths_one_line;

// 	paths_one_line = is_path(minishell, envp);
// 	minishell->exec.paths_for_search_cmd = ft_split(paths_one_line, ':');
// }

int find_first_pipe(t_token *token)
{
	t_token *tmp;
	int pipe;
	int i;

	i = 0;
	pipe = 0;
	tmp = token;
	// printf("-------------------start find pipe actuel\n");
	while (tmp != NULL && tmp->type != PIPE)
	{
		// printf("%s\n", tmp->value);
		tmp = tmp->next;
		i++;
	}
	if (tmp != NULL && tmp->type == PIPE)
	{
		// printf("%s\n", tmp->value);
		pipe = 1;
	}
	// ft_printf_fd(2, "pipe index := %d\n", i);
	// ft_printf_fd(2, "last pipe:= %d\n", lastpipe);
	// printf("-------------------end find pipe actuel\n");
	return (pipe ? i : 0);
}

/* 
Find nb of pipes, 
fill 0 or NULL,
*/
// void init_line_to_exec(t_minishell *minishell)
// {	
// 	init_pipe(minishell);
// }

/* Put values 0 or NULL in order to reuse after */
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;
	int		first_pipe;

	first_pipe = find_first_pipe(minishell->token);
	// printf("index first pipe = %d\n", first_pipe);
	if (first_pipe)
		minishell->exec.index_pipe = first_pipe;
	else
		minishell->exec.index_pipe = lst_size(minishell->token);
	// printf("nouvel index de la pipe = %d\n", minishell->exec.index_pipe);
	
	minishell->exec.input = 0;
	minishell->exec.output = 0;
	minishell->exec.index_prev_pipe = 0;
	minishell->exec.last_pipe = minishell->token;
	// minishell->exec.paths_for_search_cmd = NULL;
	// minishell->exec.envp = NULL;
	minishell->exec.pipe_lst = NULL;
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
	minishell->builtin.echo.result = NULL;
}

void	free_envp(t_minishell *minishell)
{
	free_strv(minishell->exec.envp);
	free_strv(minishell->exec.paths_for_search_cmd);
}

void	get_prompt(t_minishell *minishell)
{
	char	*base;

	base = ft_strdup("minishell$ ");
	if (minishell->builtin.echo.result)
	{
		minishell->prompt = ft_strjoin(minishell->builtin.echo.result, base);
	}
	else
		minishell->prompt = ft_strdup("minishell$ ");
	free(base);
}

int execute(t_minishell *minishell, char **envp)
{

	signal(SIGINT, handle_sigint);
	
	/*  BOUCLE WHILE  **********************************/
	//variables pour boucle while :
	char	*line;
	// bool	is_free;
	t_token	*first_token;

	first_token = NULL;
	minishell->exec.error = 0;
	minishell->prompt = NULL;
	minishell->builtin.echo.result = NULL;
	minishell->exec.envp = NULL;
	strv_dup(minishell, &minishell->exec.envp, envp);
	
	while (1)
	{
		get_prompt(minishell);
		line = readline(minishell->prompt);
		if (!line)
		{
			free(minishell->prompt);
			free_envp(minishell);
			rl_clear_history();
			printf("exit\n");
			exit (0);
		}

		if (*line)
			add_history(line);

		// PARSING ICI :************************
		line = expand_line(line, envp);
		if (separate_into_tokens(line, &first_token, minishell) != 0
				|| parse_tokens(line, &first_token, minishell) != 0)
			continue ;
		// *************************************
		
		free(line);
		
		// (void)envp;
		// (void)minishell;
		
		/* PARSING **********************************/
		if (first_token)
		{
			minishell->token = first_token;
			minishell->exec.first_token = &first_token;
		}
		else
    		minishell->token = NULL;

		init_exec(minishell);
		
		if (minishell->token)
		{
			init_pipe(minishell);
			// init_line_to_exec(minishell, envp);
			// read_all_pipes(minishell, envp);
			exec_cmds_pipe(minishell);
			// if (minishell->exec.pipe_lst->is_cmd == 1)
			// {
			// 	if (minishell->exec.nb_pipes > 0)
			// 		exec_cmds_pipe(minishell, envp);
			// 	else if (minishell->exec.pipe_lst->builtin_kind != NONE)
			// 		echo(minishell);
			// 	else
			// 		exec_cmds_pipe(minishell, envp);
			// 		// exec_cmd_no_pipe(minishell, envp);
			// }
		}
		/************************************************/
		free(minishell->prompt);
		if (minishell->token)
			ft_token_lstclear(minishell->exec.first_token);
		if (minishell->exec.pipe_lst)
			lst_pipe_clear(&minishell->exec.pipe_lst);
		
	}
	return (0);
}

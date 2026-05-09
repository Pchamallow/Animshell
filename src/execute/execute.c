/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/09 15:32:42 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void handle_sigint(int sig)*/
/*{*/
    /*struct termios oldt, newt;*/
	/*t_token	*first_token;*/

	/*first_token = NULL;*/
	/*(void)sig;*/
	/*write(1, "\nminishell$ ", 12); // réaffiche le prompt*/
/*}*/


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

// void init_paths_for_search_cmd(t_minishell *minishell, char **envp)
// {
// 	char *paths_one_line;

// 	paths_one_line = is_path(minishell, envp);
// 	minishell->exec.paths_for_search_cmd = ft_split(paths_one_line, ':');
// }

void	get_prompt(t_minishell *minishell)
{
	char	*base;

	if (minishell->prompt)
		free(minishell->prompt);
	base = ft_strdup("minishell$ ");
	if (minishell->builtin.echo.result)
	{
		minishell->prompt = ft_strjoin(minishell->builtin.echo.result, base);
		free(minishell->builtin.echo.result);
		minishell->builtin.echo.result = NULL;
		minishell->builtin.echo.for_prompt = false;
	}
	else
		minishell->prompt = ft_strdup(base);
	free(base);
}

int execute(t_minishell *minishell, char **envp)
{

	/*signal(SIGINT, handle_sigint);*/
	
	/*  BOUCLE WHILE  **********************************/
	//variables pour boucle while :
	char	*line;
	// bool	is_free;
	t_token	*first_token;

	first_token = NULL;
	minishell->exec.error = 0;
	minishell->prompt = NULL;
	minishell->builtin.echo.result = NULL;
	minishell->builtin.echo.for_prompt = false;
	minishell->exec.envp = NULL;
	minishell->exec.paths_for_search_cmd = NULL;
	strv_dup(minishell, &minishell->exec.envp, envp);
	init_pwd(minishell);
	
	while (1)
	{
		get_paths_for_cmd(minishell);
		
		set_signal_interactive();
		get_prompt(minishell);
		line = readline(minishell->prompt);
		if (!line)
		{
			free(minishell->prompt);
			free_envp(minishell);
			if (minishell->builtin.pwd.result)
				free(minishell->builtin.pwd.result);
			rl_clear_history();
			printf("exit\n");
			exit (0);
		}
		
		check_signal_value(minishell);

		if (*line)
			add_history(line);

		
		// PARSING ICI :************************
		line = expand_line(line, minishell->exec.envp, minishell);
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
			is_exit(minishell);
			exec_cmds_pipe(minishell);
		}
		/************************************************/
		if (minishell->prompt)
		{
			free(minishell->prompt);
			minishell->prompt = NULL;
		}
			
		free_heredoc(minishell);
		
		if (minishell->token)
			ft_token_lstclear(minishell->exec.first_token);
		if (minishell->exec.pipe_lst)
			lst_pipe_clear(&minishell->exec.pipe_lst);
			
	}
	return (0);
}

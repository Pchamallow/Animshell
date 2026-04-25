/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/24 22:29:43 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    /*struct termios oldt, newt;*/
	/*t_token	*first_token;*/

	/*first_token = NULL;*/
	(void)sig;
	rl_replace_line("",0);//remplace complètement le contenu actuel de la ligne en cours d’édition
	rl_on_new_line();//tells the update routine that we have moved onto a new empty line
	rl_redisplay();//met a jour le display
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
		current->error = 0;
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
find the PATH (split each little paths)
*/
void init_line_to_exec(t_minishell *minishell, char **envp)
{	
	// env -i -> retirer envp
	strv_dup(minishell, &minishell->exec.envp, envp);
	minishell->exec.nb_pipes = nb_pipes(minishell->token);
	init_pipe(minishell);
	// init_paths_for_search_cmd(minishell, envp);
}

/* Put values 0 or NULL in order to reuse after */
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;
	int		first_pipe;

	first_pipe = find_first_pipe(minishell->token);
	minishell->exec.index_pipe = first_pipe;
	// printf("nouvel index de la pipe = %d\n", minishell->exec.index_pipe);

	minishell->exec.input = 0;
	minishell->exec.output = 0;
	minishell->exec.last_pipe = minishell->token;
	minishell->exec.paths_for_search_cmd = NULL;
	minishell->exec.envp = NULL;
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

void	set_signal_default(void)
{
	struct sigaction sa_default;

	ft_bzero(&sa_default, sizeof(struct sigaction));
	sa_default.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}

int execute(t_minishell *minishell, char **envp)
{

	
	/*  BOUCLE WHILE  **********************************/
	//variables pour boucle while :
	char	*line;
	// bool	is_free;
	t_token	*first_token;

	first_token = NULL;
	minishell->exec.error = 0;
	
	
	while (1)
	{
		signal(SIGINT, handle_sigint);
		line = readline("minishell$ ");
		if (!line)
		{
			// free_all(minishell);
			// free_strv(minishell->exec.envp);
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
			set_signal_default();
			init_line_to_exec(minishell, envp);
			// read_all_pipes(minishell, envp);
			exec_cmds_pipe(minishell, envp);
			// if (minishell->exec.pipe_lst->is_cmd == 1)
			// {
			// 	if (minishell->exec.nb_pipes > 0)
			// 		exec_cmds_pipe(minishell, envp);
			// 	else if (minishell->exec.pipe_lst->built_in != NONE)
			// 		echo(minishell);
			// 	else
			// 		exec_cmds_pipe(minishell, envp);
			// 		// exec_cmd_no_pipe(minishell, envp);
			// }
		}
		/************************************************/
		
		free_all(minishell);
		
	}
	return (0);
}

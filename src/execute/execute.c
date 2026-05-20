/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/20 16:00:01 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	nb_pipes(t_token *first)
{
	t_token	*token;
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

static void	exit_ctrl_d(t_minishell *minishell)
{
	free(minishell->prompt);
	free_envp(minishell);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	rl_clear_history();
	printf("exit\n");
	exit(minishell->exec.error);
}

static bool	parsing_syntax_ok(char *line, t_minishell *minishell)
{
	line = expand_line(line, minishell->exec.envp, minishell);
	if (separate_into_tokens(line, minishell->exec.first_token, minishell) != 0
		|| parse_tokens(line, minishell->exec.first_token, minishell) != 0)
		return (false);
	free(line);
	return (true);
}

static void	reinit_minishell(t_minishell *minishell)
{
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
	if (minishell->builtin.cd.result)
		free(minishell->builtin.cd.result);
}

int	execute(t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		set_signal_interactive();
		get_paths_for_cmd(minishell);
		get_prompt(minishell);
		line = readline(minishell->prompt);
		if (!line)
			exit_ctrl_d(minishell);
		check_signal_value(minishell);
		if (*line)
			add_history(line);
		if (!parsing_syntax_ok(line, minishell))
			continue ;
		init_exec(minishell);// voir si on peut optimiser avec bzero 
		if (minishell->token)
		{
			init_pipe(minishell);
			exec_cmds_pipe(minishell);
		}
		reinit_minishell(minishell);
	}
	return (0);
}

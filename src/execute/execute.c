/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 12:04:53 by pswirgie         ###   ########.fr       */
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

static void	exit_ctrl_d(t_minishell *minishell)
{
	free_envp(minishell);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	rl_clear_history();
	printf("exit\n");
	exit(minishell->exec.error);
}

static void	reinit_minishell(t_minishell *minishell)
{
	free_heredoc(minishell);
	if (minishell->token)
		ft_token_lstclear(minishell->exec.first_token);
	if (minishell->exec.pipe_lst)
		lst_pipe_clear(&minishell->exec.pipe_lst);
	if (minishell->builtin.cd.result)
		free(minishell->builtin.cd.result);
	minishell->exec.pipe_actual = 0;
}

void	execute(t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		set_signal_interactive();
		get_paths_for_cmd(minishell);
		line = readline("minishell$ ");
		if (!line)
			exit_ctrl_d(minishell);
		check_signal_value(minishell);
		if (*line)
			add_history(line);
		if (!parsing_syntax_ok(line, minishell))
			continue ;
		init_exec(minishell);
		if (minishell->token)
		{
			init_pipe(minishell);
			exec_cmds_pipe(minishell);
		}
		reinit_minishell(minishell);
	}
	return ;
}

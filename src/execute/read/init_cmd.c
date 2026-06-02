/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 15:31:54 by stkloutz          #+#    #+#             */
/*   Updated: 2026/06/02 12:14:17 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_built_in(t_pipe *pipe, t_token *token)
{
	int	len;

	len = ft_strlen(token->value);
	if (ft_strncmp(token->value, "echo", len) == 0)
		pipe->builtin_kind = IS_ECHO;
	else if (ft_strncmp(token->value, "cd", len) == 0)
		pipe->builtin_kind = CD;
	else if (ft_strncmp(token->value, "pwd", len) == 0)
		pipe->builtin_kind = PWD;
	else if (ft_strncmp(token->value, "export", len) == 0)
		pipe->builtin_kind = EXPORT;
	else if (ft_strncmp(token->value, "unset", len) == 0)
		pipe->builtin_kind = UNSET;
	else if (ft_strncmp(token->value, "env", len) == 0)
		pipe->builtin_kind = ENV;
	else if (ft_strncmp(token->value, "exit", len) == 0)
		pipe->builtin_kind = EXIT;
}

static bool	is_cmd_found(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	int		invalid_cmd;

	invalid_cmd = path_cmd(minishell, pipe, token);
	if (!invalid_cmd)
	{
		pipe->cmd = token;
		pipe->is_cmd = 1;
	}
	else if (invalid_cmd == 1)
		token->type = WORD;
	else
	{
		token->type = WORD;
		if (!pipe->infile_error && !pipe->outfile_error)
			ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
		minishell->exec.error = 127;
		return (false);
	}
	return (true);
}

/*
Init_cmd

A. IS_CMD
path_cmd :
1. is an environnement variable PATH ?
2. is a explicit path, absolute path ?
Or path is to search
3. is cmd valid ?
	Path_cmd
		1. is an environnement variable PATH ?
		2. is a explicit path, absolute path ?
		Or path is to search
		3. is cmd valid ?

B. IS_BUILT_IN
1. what built in is ?

C. if there is a command and at least one arg
= initialisation tab args
*/
int	init_cmd(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*token;
	int		nb_cmd_args;

	token = minishell->exec.last_pipe;
	nb_cmd_args = nb_args(token);
	while (token)
	{
		if (token->type == PIPE)
			break ;
		else if (token->type == IS_CMD)
		{
			if (!is_cmd_found(minishell, pipe, token))
				return (1);
		}
		else if (token->type == IS_BUILT_IN)
		{
			is_built_in(pipe, token);
			pipe->cmd = token;
			pipe->is_cmd = 1;
		}
		token = token->next;
	}
	if (pipe->is_cmd && nb_cmd_args > 0)
		init_cmd_args(minishell, pipe, nb_cmd_args);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/25 13:33:58 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	next_pipe(t_minishell *minishell, t_token *token)
{
	/*t_token	*tmp;*/
	int		i;
	int		is_pipe;

	i = 0;
	is_pipe = 0;
	/*tmp = token;*/
	while (token)
	{
		if (i > 0 && token->type == PIPE)
		{
			is_pipe = 1;
			if (token->next)
				token = token->next;
			break ;
		}
		token = token->next;
		i++;
	}
	if (token)
	{
		if (is_pipe)
		{
			minishell->exec.last_pipe = token;
			return (i);
		}
		else
			minishell->exec.index_pipe = -1;
	}
	return (0);
}

int	nb_args(t_token *token)
{
	t_token	*tmp;
	int		args;

	args = 0;
	tmp = token;
	if (tmp->type == PIPE)
		tmp = tmp->next;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == IS_ARG)
		{
			if (tmp->value && tmp->value[0] != '\0')
				args++;
		}
		tmp = tmp->next;
	}
	return (args);
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
static int	init_cmd(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*token;
	int		nb_cmd_args;
	int		invalid_cmd;

	token = minishell->exec.last_pipe;
	nb_cmd_args = nb_args(token);
	while (token)
	{
		if (token->type == PIPE)
			break ;
		else if (token->type == IS_CMD)
		{
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
					ft_printf_fd(2, "minishell: %s: command not found\n",
						token->value);
				minishell->exec.error = 127;
				return (1);
			}
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

/*
if token is an argument and we have a command
-> add to char **cmd_args
*/
void	read_args(t_minishell *minishell, t_token *token, t_pipe *pipe)
{
	int	args;

	args = 0;
	while (token)
	{
		if (token->type == PIPE)
			break ;
		else if (token->type == IS_ARG)
		{
			if (pipe->is_cmd == 1 && token->value && token->value[0] != '\0')
			{
				add_args(minishell, pipe, token);
				args++;
			}
		}
		token = token->next;
	}
	pipe->nb_args = args;
}

/*
1. read files
- error files before command

2. init command

Pipe :
-> input pipe : input from previous pipe
-> ouput pipe : output to next pipe

*/
int	read_tokens(t_minishell *minishell, t_pipe *pipe, int fd)
{
	t_token	*token;
	int		index_next_pipe;

	token = minishell->exec.last_pipe;
	if (find_input_output(minishell, pipe, fd) || init_cmd(minishell, pipe))
	{
		minishell->exec.index_prev_pipe = minishell->exec.index_pipe;
		index_next_pipe = next_pipe(minishell, token);
		minishell->exec.index_pipe += index_next_pipe;
		return (-1);
	}
	read_args(minishell, token, pipe);
	minishell->exec.index_prev_pipe = minishell->exec.index_pipe;
	index_next_pipe = next_pipe(minishell, token);
	minishell->exec.index_pipe = minishell->exec.index_pipe + index_next_pipe;
	return (0);
}

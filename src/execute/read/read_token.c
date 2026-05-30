/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/30 16:49:56 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_pipe(t_token *token, int *is_pipe, int *i)
{
	while (token)
	{
		if (*i > 0 && token->type == PIPE)
		{
			*is_pipe = 1;
			if (token->next)
				token = token->next;
			return (token);
		}
		token = token->next;
		(*i)++;
	}
	return (token);
}

int	next_pipe(t_minishell *minishell, t_token *token)
{
	int		i;
	int		is_pipe;

	i = 0;
	is_pipe = 0;
	token = get_next_pipe(token, &is_pipe, &i);
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

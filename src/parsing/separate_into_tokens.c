/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_into_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:36:09 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/18 22:25:52 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

bool	is_separator(char c)
{
	if (ft_strchr(" \t|<>\"\'", c) == NULL)
		return (false);
	return (true);
}

static int	set_token_type(char *line, t_token **token_list,
		t_minishell *minishell, int *i)
{
	if (line[*i] == '\"' || line[*i] == '\'')
	{
		if (handle_quotes(line, token_list, i, minishell) != 0)
			return (1);
	}
	else if (line[*i] == '|')
		handle_pipe(line, token_list, i, minishell);
	else if (line[*i] == '>' || line[*i] == '<')
		handle_redirection(line, i, line[*i], minishell);
	else if (line[*i] && !is_separator(line[*i]))
		handle_words_no_quotes(line, token_list, i, minishell);
	handle_spaces(line, token_list, i, minishell);
	return (0);
}

/*	******************************************************		*/
/*	separate_into_tokens:										*/
/* 1.Trims spaces at the beginning of the input line			*/
/* 2.Separates the line into tokens:							*/
/* Types of tokens:												*/
/*		- SPACE: spaces, tabs -> become 1 space					*/
/*		- PIPE: |												*/
/*		- REDIRECTION: <, <<, >, >>								*/
/*		- WORD: 												*/
/*			- a sequence of characters enclosed by "" or ''		*/
/*			- a sequence of characters separated by spaces,		*/
/* 				tabs, or any character listed above				*/
/*	NOTE: only handle_quotes can return an error				*/
/*	-> in that case, separate_into_tokens returns 1				*/
/*	******************************************************		*/
int	separate_into_tokens(char *line, t_token **token_list,
		t_minishell *minishell)
{
	int	i;

	if (!line || line[0] == '0')
	{
		if (line)
			free(line);
		return (1);
	}
	i = 0;
	while (is_whitespace(line[i]))
		i++;
	while (line[i])
	{
		if (set_token_type(line, token_list, minishell, &i) != 0)
			return (1);
	}
	return (0);
}

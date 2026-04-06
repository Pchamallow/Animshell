/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_into_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:36:09 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/06 21:21:10 by stkloutz         ###   ########.fr       */
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

void	print_tokens_types(t_token *token)// pour tester
{
	char *str[] = {"word", "is_cmd", "is_built_in", "is_arg", "is_filename",
		"is_delimiter", "space", "pipe", "redirection", "is input",
		"is_output", "is_append", "heredoc"};
	/*char *quote[] = {"no", "single", "double"};*/
	while (token)
	{
		/*ft_printf_fd(1, "%s	type=%s quote=%s\n", token->value,*/
				/*str[token->type], quote[token->quote]);*/
		ft_printf_fd(1, "%s		type=%s\n", token->value,
				str[token->type]);
		token = token->next;
	}
}

void	print_tokens(t_token *token)// pour tester
{
	int	something_to_write;

	something_to_write = 0;
	while (token)
	{
		ft_printf_fd(1, "%s", token->value);
		something_to_write = 1;
		token = token->next;
	}
	if (something_to_write)
		ft_printf_fd(1, "\n");
}

/*	************************************************	*/
/*		This function :									*/
/*1.Trim spaces at the beginning of the input line		*/
/*2.Separate the line into tokens :						*/
/* A token is:											*/
/*		- spaces, tabs -> become 1 space				*/
/*		- a sequence of characters enclosed by "" or ''	*/
/*		- |												*/
/*		- <, <<, >, >>									*/
/*		- a sequence of characters separated by spaces,	*/
/* 		tabs, or any character listed above				*/
/*	************************************************	*/
int	separate_into_tokens(char *line, t_token **token_list)
{
	int	i;

	i = 0;
	while (is_whitespace(line[i]))
		i++;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
			handle_quotes(line, token_list, &i, line[i]);
		handle_spaces(line, token_list, &i);
		if (line[i] == '|')
			handle_pipe(line, token_list, &i);
		handle_spaces(line, token_list, &i);
		if (line[i] == '>' || line[i] == '<')
			handle_redirection(line, token_list, &i, line[i]);
		handle_spaces(line, token_list, &i);
		if (line[i] && !is_separator(line[i]))
			handle_words_no_quotes(line, token_list, &i);
		handle_spaces(line, token_list, &i);
	}
	//tests : *******
	/*print_tokens_types(*token_list);*/
	print_tokens(*token_list);
	// **************
	return (0);
}

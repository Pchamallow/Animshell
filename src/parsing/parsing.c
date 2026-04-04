/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:36:09 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/04 18:26:08 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_token_new(char *str, t_token_type token_type)
{
	t_token	*token;

	token = malloc(sizeof (t_token));
	if (!token)
		return (NULL);
	ft_bzero(token, sizeof(t_token));
	token->type = token_type;
	token->value = str;
	token->next = NULL;
	return (token);
}

bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

/* 1 token is:											*/
/*- all characters between "" or ''						*/
/*- |													*/
/*- < or <<												*/
/*- > or >>												*/
/*- =													*/
/*- a sequence of characters separated by spaces, tabs,	*/
/* or any character listed above						*/
int	count_tokens(char *line)
{
	int nb_tokens;
	int	i;

	nb_tokens = 0;
	while (line[i])
	{
		//trim spaces :
		while (is_whitespace(line[i]))
			i++;
		//double quotes :
		if (line[i] == '"')
		{
			while (line[i] != '"')
			{
				i++;
				if (line[i] == '\0')
				{
					ft_printf_fd(2, "Error: unclosed quotes");
					return(1);
				}
			}
			nb_tokens++;
			i++;
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//single quotes :
		if (line[i] == ''')
		{
			while (line[i] != ''')
			{
				i++;
				if (line[i] == '\0')
				{
					ft_printf_fd(2, "Error: unclosed quotes");
					return(-1);
				}
			}
			nb_tokens++;
			i++;
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//pipes :
		if (line[i] && line[i] == '|')
		{
			nb_tokens++;
			i++;
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//redirections < :
		if (line[i] && line[i] == '<')
		{
			nb_tokens++;
			i++;
				if (line[i] == '<')
					i++;
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//redirections > :
		if (line[i] && line[i] == '>')
		{
			nb_tokens++;
			i++;
				if (line[i] == '>')
					i++;
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//words not enclosed with quotes:
		if (line[i])
		{
			nb_tokens++;
			while (line[i] && !is_separator(line[i]))
				i++;
		}

	}
	return(nb_tokens);
}

t_token	*separate_into_tokens(char *line, t_token **token_list)
{
	int	i;
	int	start;
	int	len;

	while (line[i])
	{
		//trim spaces
		while (is_whitespace(line[i]))
			i++;
		//if quote
		if (line[i] == '"')
		{
			start = i + 1; // + 1 pour pas avoir de quote
			while (line[i] != '"')
			{
				i++;
				if (line[i] == '\0')
				{
					ft_printf_fd(2, "Error: unclosed quotes");
					return(1);
				}
			}
			i++;
			len = i - start;
			//lst_add_back(token_list, ft_token_new(ft_substr, WORD));
			//trim spaces :
			while (is_whitespace(line[i]))
				i++;
		}
		//if pipe
		//if redirection
		//if words not enclosed with quotes
		i++;
	}

}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:05:44 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/13 11:52:03 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	add_quote_type stores in @token->quote	*/
/*	the type of quotes						*/
/*	used in the original input and			*/
/*	removed by the handle_quotes function	*/
static void	add_quote_type(t_token *token, char quote)
{
	if (quote == '\'')
		token->quote = SINGLE;
	if (quote == '\"')
		token->quote = DOUBLE;
}

static bool	is_quote_closed(char *line, int *index, char quote)
{
	int	i;

	i = *index;
	if (!line[i])
		return (false);
	while (line[i] != quote)
	{
		i++;
		if (!line[i])
			return (false);
	}
	*index = i;
	return (true);
}

/*	handle_quotes creates a token							*/
/*	with the text inside quotes,							*/
/*	removing the quotes.									*/
/*	It stops and returns 1 in case of unclosed quotes		*/
int	handle_quotes(char *line, t_token **token_list, int *index,
		t_minishell *minishell)
{
	int		i;
	int		start;
	int		len;
	char	quote;

	i = *index;
	quote = line[i];
	i++;//pour enlever le quote de debut
	start = i;
	/*i++;//pour garder le quote de debut*/
	if (!is_quote_closed(line, &i, quote))
	{
		error_quote(line, token_list, minishell);
		return (1);
	}
	/*i++;//pour garder le quote de fin*/
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line);
	add_quote_type(ft_token_last(*token_list), quote);
	i++;//pour passer le quote de fin
	*index = i;
	return (0);
}

void	handle_pipe(char *line, t_token **token_list, int *index)
{
	int	i;

	i = *index;
	ft_token_add_back(token_list,
		ft_token_new(ft_strdup("|"), PIPE), line);
	i++;
	*index = i;
}

void	handle_redirection(char *line, t_token **token_list,
		int *index, char angle_bracket)
{
	int	i;
	int	start;
	int	len;

	i = *index;
	start = i;
	len = 1;
	if (line[i + 1] == angle_bracket)
	{
		len++;
		i++;
	}
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), REDIRECTION), line);
	i++;
	*index = i;
}

/*	handle_words_no_quotes creates a token			*/
/*	with a word not enclosed with quotes.			*/
/*	A word is a sequence of characters separated 	*/
/*	by spaces, tabs, <, <<, >, >>, |, " or '		*/
void	handle_words_no_quotes(char *line, t_token **token_list, int *index)
{
	int	i;
	int	start;
	int	len;

	i = *index;
	start = i;
	while (line[i] && !is_separator(line[i]))
		i++;
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line);
	*index = i;
}

/*	handle_spaces creates a token with one space	*/
/*	each time spaces and/or tabs are encountered.	*/
/*	No matter the number of spaces/tabs,			*/
/*	the token will contain only one space			*/
void	handle_spaces(char *line, t_token **token_list, int *index)
{
	int	i;

	i = *index;
	if (is_whitespace(line[i]))
	{
		ft_token_add_back(token_list,
			ft_token_new(ft_strdup(" "), ONE_SPACE), line);
	}
	while (is_whitespace(line[i]))
		i++;
	*index = i;
}

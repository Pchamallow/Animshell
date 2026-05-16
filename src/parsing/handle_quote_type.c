/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 19:03:34 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 19:04:32 by stkloutz         ###   ########.fr       */
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
	i++;
	start = i;
	if (!is_quote_closed(line, &i, quote))
	{
		error_quote(line, token_list, minishell);
		return (1);
	}
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line, minishell);
	add_quote_type(ft_token_last(*token_list), quote);
	i++;
	*index = i;
	return (0);
}

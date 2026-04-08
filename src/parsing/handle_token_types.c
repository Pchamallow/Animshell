/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:05:44 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/08 19:40:59 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	This function stores in token->quote	*/
/*	the type of quotes						*/
/*	used in the original input and			*/
/*	removed by the handle_quotes function	*/
void	add_quote_type(t_token *token, char quote)
{
	if (quote == '\'')
		token->quote = SINGLE;
	if (quote == '\"')
		token->quote = DOUBLE;
}

/*	This function creates a token							*/
/*	with the text inside quotes,							*/
/*	removing the quotes.									*/
/*	It prints an error and exits in case of unclosed quotes	*/
void	handle_quotes(char *line, t_token **token_list, int *index, char quote)
{
	int	i;
	int	start;
	int	len;

	i = *index;
	i++;//pour enlever le quote de debut
	start = i;
	while (line[i] != quote)
	{
		i++;
		if (line[i] == '\0')
		{
			ft_printf_fd(2, "Error: unclosed quotes\n");
			ft_token_lstclear(token_list);
			free(line);
			exit(2);//pas besoin de tout quitter ici, juste afficher une erreur et revenir a l'input
		}
	}
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line);
	add_quote_type(ft_token_last(*token_list), quote);
	i++;//pour passer le quote de fin
	*index = i;
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

/*	This function creates a token			*/
/*	with the redirection operator.			*/
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

/*	This function creates a token					*/
/*	with a word not enclosed with quotes.			*/
/*	A word is a sequence of characters separated 	*/
/*	by spaces, tabs, <, <<, >, >>, |, ", '			*/
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

/*	This function creates a token with one space	*/
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

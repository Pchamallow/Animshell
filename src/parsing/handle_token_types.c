/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:05:44 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 19:07:45 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe(char *line, t_token **token_list, int *index,
		t_minishell *minishell)
{
	int	i;

	i = *index;
	ft_token_add_back(token_list,
		ft_token_new(ft_strdup("|"), PIPE), line, minishell);
	i++;
	*index = i;
}

void	handle_redirection(char *line, int *index, char angle_bracket,
		t_minishell *minishell)
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
	ft_token_add_back(minishell->exec.first_token,
		ft_token_new(ft_substr(line, start, len),
			REDIRECTION), line, minishell);
	i++;
	*index = i;
}

/*	handle_words_no_quotes creates a token			*/
/*	with a word not enclosed with quotes.			*/
/*	A word is a sequence of characters separated 	*/
/*	by spaces, tabs, <, <<, >, >>, |, " or '		*/
void	handle_words_no_quotes(char *line, t_token **token_list, int *index,
		t_minishell *minishell)
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
		ft_token_new(ft_substr(line, start, len), WORD), line, minishell);
	*index = i;
}

/*	handle_spaces creates a token with one space	*/
/*	each time spaces and/or tabs are encountered.	*/
/*	No matter the number of spaces/tabs,			*/
/*	the token will contain only one space			*/
void	handle_spaces(char *line, t_token **token_list, int *index,
		t_minishell *minishell)
{
	int	i;

	i = *index;
	if (is_whitespace(line[i]))
	{
		ft_token_add_back(token_list,
			ft_token_new(ft_strdup(" "), ONE_SPACE), line, minishell);
	}
	while (is_whitespace(line[i]))
		i++;
	*index = i;
}

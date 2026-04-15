/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:05:44 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/15 11:02:51 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	add_quote_type stores in @token->quote	*/
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

{
	int	i;

	i = *index;
	while (line[i] != quote)
	{
		i++;
		if (line[i] == '\0')
		{
			ft_printf_fd(2, "Error: unclosed quotes\n");
			ft_token_lstclear(token_list);
			free(line);
			return (false);
		}
	}
	*index = i;
	return (true);
}

void	count_env_var(char *line, int len, int i, char **envp)
{
	int	j;
	int	wd_len;
	int	count;

	count = 0;
	while (i < len)
	{
		if (line[i] == '$' && i + 1 < len)
		{
			if (line[i + 1] == '?')
			{
				//$?
				//if (exit_error < 10)
				//	count += 1;
				//else if (exit_error >= 100)
				//	count += 3;
				//else
				//	exit error: format d'erreur inattendu
				//i += 2;
			}
			else if (!is_whitespace(line[i + 1]))
			{
				wd_len = 0;
				//count $ENV:
				//1.find word:
				//i++;
				//while (i + wd_len < len && !is_whitespace(line[i + wd_len])
				//			&& line[i + wd_len] != '$')
				//	wd_len++;
				//2.find word in envp:
				//j = 0;
				//while (envp[j] && ft_strncmp(line + i, envp[j], wd_len) != 0)
				//	j++;
				//3.count char:
				//if (envp[j])
				//{
				//	count += count_env_var(envp[j] + wd_len + 1,
				//		ft_strlen(envp[j] + wd_len + 1, 0, envp));
				//}
			}
		}
		i++;
	}
}

/*	handle_quotes creates a token							*/
/*	with the text inside quotes,							*/
/*	keeping the quotes.										*/
/*	It stops and returns 1 in case of unclosed quotes		*/
int	handle_quotes(char *line, t_token **token_list, int *index, char quote)
{
	int	i;
	int	start;
	int	len;

	i = *index;
	/*i++;//pour enlever le quote de debut*/
	start = i;
	i++;//pour garder le quote de debut
	if (!is_quote_closed(line, token_list, &i, quote))
		return (1);
	i++;//pour garder le quote de fin
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line);
	add_quote_type(ft_token_last(*token_list), quote);
	/*i++;//pour passer le quote de fin*/
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

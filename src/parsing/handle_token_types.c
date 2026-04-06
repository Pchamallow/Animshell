/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 19:05:44 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/06 17:28:51 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_quote_type(t_token *token, char quote)
{
	if (quote == '\'')
		token->quote = SINGLE;
	if (quote == '\"')
		token->quote = DOUBLE;
}

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
			exit(2);//pas besoin de tout quitter ici, juste afficher une erreur
		}
	}
	len = i - start;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), WORD), line);
	add_quote_type(ft_token_last(*token_list), quote);
	i++;//pour enlever le quote de fin
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

void	handle_redirection(char *line, t_token **token_list,
		int *index, char angle_bracket)
{
	int	i;
	int	start;
	int	len;
	int	type;

	i = *index;
	start = i;
	len = 1;
	if (line[i + 1] == angle_bracket)
	{
		len++;
		i++;
	}
	if (angle_bracket == '<' && len == 2)
		type = HEREDOC;
	else
		type = REDIRECTION;
	ft_token_add_back(token_list,
		ft_token_new(ft_substr(line, start, len), type), line);
	i++;
	*index = i;
}

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

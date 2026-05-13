/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 22:10:40 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/12 22:55:55 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** find_env_var
** looks for ENV VAR in @line
** - if a ENV VAR is found,
** it returns @i the index of @line
** - if no ENV VAR is found
** it returns -1
*/
int	find_env_var(char *line, int len, t_quote_type *quote)
{
	int				i;

	i = 0;
	while (i < len - 1)
	{
		toggle_quote(line[i], quote);
		if (*quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
			&& !is_separator(line[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

int	get_exit_status_len(int exit_status)
{
	int	count;

	if (exit_status < 0 || exit_status > 255)
		exit_status = 255;
	count = 1;
	while (exit_status >= 10)
	{
		exit_status = exit_status / 10;
		count++;
	}
	return (count);
}

int	count_words(char *str)
{
	int	count;
	int	i;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		if (str[i])
			count++;
		while (str[i] && !is_whitespace(str[i]))
			i++;
	}
	return (count);
}

/*
** add_var_value
** returns the number of chars 
** contained in one specific ENV VAR value.
** If the ENV VAR contains quotes inside,
** it adds 2 quotes per word
** in order to keep the inside quotes intact
*/
int	add_var_value_len(char **envp, char *line, int count, t_quote_type quote)
{
	int	wd_len;
	int	j;

	wd_len = get_var_name_len(line);
	j = get_var(line, envp, wd_len);
	if (j != -1 && envp[j] != NULL)
	{
		count += (ft_strlen(envp[j]) - (wd_len + 1));
		if (quote_found(envp[j]) && quote == NO)
			count += (2 * count_words(envp[j]));
	}
	count -= (wd_len + 1);
	return (count);
}

/*
** count_total_char
** returns the number of chars of the string
** with all ENV VAR "$name" replaced by their values
*/
int	count_total_char(char *line, int len, t_minishell *minishell)
{
	int				i;
	int				count;
	t_quote_type	quote;

	count = len;
	i = 0;
	quote = NO;
	while (i < len)
	{
		toggle_quote(line[i], &quote);
		if (quote != SINGLE && line[i] == '$' && line[i + 1] == '?')
			count += get_exit_status_len(minishell->exec.error) - 2;
		else if (quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
			&& line[i + 1] != '?' && !is_separator(line[i + 1]))
		{
			i++;
			count = add_var_value_len(minishell->exec.envp, line + i,
					count, quote);
		}
		i++;
	}
	return (count);
}

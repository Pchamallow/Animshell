/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 22:10:40 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/10 22:37:37 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			count += 2;
	}
	count -= (wd_len + 1);
	return (count);
}

/*
** count_total_char
** returns the number of chars of the string
** with all env variables "$name" replaced by their values
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

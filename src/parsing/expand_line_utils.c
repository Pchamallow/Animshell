/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 22:15:28 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/10 22:17:39 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	toggle_quote(char c, t_quote_type *quote)
{
	if (c == '\"')
	{
		if (*quote == NO)
			*quote = DOUBLE;
		else if (*quote == DOUBLE)
			*quote = NO;
	}
	if (c == '\'')
	{
		if (*quote == NO)
			*quote = SINGLE;
		else if (*quote == SINGLE)
			*quote = NO;
	}
}

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

int	get_var_name_len(char *line)
{
	int	wd_len;
	int	len;
	int	i;

	wd_len = 0;
	i = 0;
	len = ft_strlen(line);
	while (i + wd_len < len && !is_separator(line[i + wd_len])
		&& line[i + wd_len] != '$')
		wd_len++;
	return (wd_len);
}

int	get_var(char *line, char **envp, int wd_len)
{
	int	j;

	if (!envp)
		return (-1);
	j = 0;
	while (envp[j] && ft_strncmp(line, envp[j], wd_len) != 0)
		j++;
	return (j);
}

bool	quote_found(char *str)
{
	int	i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

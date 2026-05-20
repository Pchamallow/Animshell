/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 22:15:28 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/20 21:04:00 by stkloutz         ###   ########.fr       */
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

int	get_var_name_len(char *line)
{
	int	wd_len;
	int	len;
	int	i;

	wd_len = 0;
	i = 0;
	len = ft_strlen(line);
	while (i + wd_len < len
		&& (ft_isalnum(line[i + wd_len]) || line[i + wd_len] == '_'))
		wd_len++;
	return (wd_len);
}

static bool	var_cmp(char *line, char *str, int wd_len)
{
	int	i;

	i = 0;
	while ((line[i] || str[i]) && i < wd_len)
	{
		if (line[i] != str[i])
			return (false);
		i++;
	}
	if (str[i] && str[i] != '=')
		return (false);
	return (true);
}

int	get_var(char *line, char **envp, int wd_len)
{
	int	j;

	if (!envp)
		return (-1);
	j = 0;
	while (envp[j] && !var_cmp(line, envp[j], wd_len))
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

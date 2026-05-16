/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 22:15:28 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 09:41:02 by stkloutz         ###   ########.fr       */
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
		&& (ft_isalnum(line[i + wd_len]) || line[i + wd_len] == '_')
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

char	chose_quote(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && !is_whitespace(str[i]))
	{
		if (str[i] == '\'')
			return ('\"');
		if (str[i] == '\"')
			return ('\'');
		i++;
	}
	return ('\"');
}

void	ft_strlcat_add_quotes(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	if (dst)
		i = ft_strlen(dst);
	j = 0;
	while ((i + j) < size && src[j])
	{
		quote = chose_quote(src + j);
		dst[i + j] = quote;
		i++;
		while ((i + j) < size && src[j] && !is_whitespace(src[j]))
		{
			dst[i + j] = src[j];
			j++;
		}
		dst[i + j] = quote;
		i++;
		while ((i + j) < size && src[j] && is_whitespace(src[j]))
		{
			dst[i + j] = src[j];
			j++;
		}
	}
	dst[i + j] = '\0';
}

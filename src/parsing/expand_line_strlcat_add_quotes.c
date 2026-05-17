/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line_strlcat_add_quotes.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 17:50:21 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 19:00:21 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	chose_quote(const char *str)
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

static void	copy_src_to_dst(char *dst, const char *src, size_t *i, size_t *j)
{
	dst[*i + *j] = src[*j];
	(*j)++;
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
			copy_src_to_dst(dst, src, &i, &j);
		dst[i + j] = quote;
		i++;
		while ((i + j) < size && src[j] && is_whitespace(src[j]))
			copy_src_to_dst(dst, src, &i, &j);
	}
	dst[i + j] = '\0';
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:28:00 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 17:46:36 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	len_s1;
	int	len_s2;
	int	result;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 > len_s2)
		result = ft_strncmp(s1, s2, len_s1);
	else
		result = ft_strncmp(s1, s2, len_s2);
	return (result);
}

int	cpy_strvindex(char **result, char **src, char *search)
{
	int	index;
	int	len_search;

	len_search = ft_strlen(search);
	index = strv_searchindex(src, search);
	if (index == -1)
		return (-1);
	*result = ft_substr(src[index], len_search,
			ft_strlen(src[index]) - len_search);
	if (!*result)
		return (1);
	return (0);
}

int	strv_searchindex(char **strv, char *search)
{
	int	i;
	int	len;

	i = 0;
	if (!*strv)
		return (-1);
	len = ft_strlen(search);
	while (strv[i])
	{
		if (ft_strnstr(strv[i], search, len) != NULL)
			return (i);
		i++;
	}
	return (-1);
}

int	str_copy_and_free(char **src, char **dst)
{
	char	*tmp;

	tmp = ft_strdup(*src);
	if (!tmp)
		return (1);
	free(*dst);
	*dst = tmp;
	return (0);
}

int	has_alpha(char *str)
{
	int	alpha;
	int	end;

	alpha = 0;
	end = ft_strlen(str) - 1;
	if (end <= 0)
		return (0);
	while (str[end] && end > 0)
	{
		if (ft_isalpha(str[end]))
		{
			alpha = 1;
			break ;
		}
		end--;
	}
	return (alpha);
}

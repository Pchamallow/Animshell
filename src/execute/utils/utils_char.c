/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:11:40 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 14:55:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_chr(char *str, char c, bool followed)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			while (str[i] == c)
			{
				count++;
				i++;
			}
			if (followed == true)
				return (count);
		}
		i++;
	}
	if (followed)
		return (count);
	return (0);
}

int	index_lastchar(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] && str[i] != c)
		i--;
	return (i);
}

int	join_oldnew(char **old, char **new)
{
	char	*new_path;

	new_path = ft_strjoin(*old, *new);
	if (!new_path)
		return (1);
	free(*new);
	*new = new_path;
	return (0);
}

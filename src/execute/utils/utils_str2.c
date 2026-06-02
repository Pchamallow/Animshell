/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:56:04 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 12:07:43 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_iswhitespaces(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (!is_whitespace(s[i]))
			return (0);
		i++;
	}
	return (1);
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

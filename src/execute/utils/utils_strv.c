/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:29:03 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 12:27:29 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strv(char **array)
{
	int	i;

	i = 0;
	if (!array || !array[i])
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	is_safe_strvlen(char **s, int len)
{
	int	safe;
	int	i;

	i = 0;
	safe = 1;
	while (i < len)
	{
		if (!s[i])
		{
			free(s[i]);
			safe = 0;
		}
		i++;
	}
	if (!safe)
		return (0);
	return (1);
}

// verifier si cest utiliser
int	memcpy_strv(char **dst, char **src, int max)
{
	int	i;

	i = 0;
	if (!src || !*src)
		return (1);
	while ((src[i] && max == 0) || (i < max))
	{
		dst[i] = src[i];
		i++;
	}
	return (0);
}

int	strvlen(char **array)
{
	int	i;

	i = 0;
	if (!array || !*array)
		return (-1);
	while (array[i])
		i++;
	return (i);
}

void	free_strv_len(char **array, int len)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (i < len)
	{
		if (array[i])
			free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

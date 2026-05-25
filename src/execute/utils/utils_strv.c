/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:29:03 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/25 08:36:37 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

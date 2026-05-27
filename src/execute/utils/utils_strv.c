/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:29:03 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 16:57:05 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strv(char **array)
{
	int	i;

	i = 0;
	// if (!array || !array[i])
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

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

char	**strv_dup(t_minishell *minishell, char **src)
{
	char	**dst;
	int		len;
	int		i;

	i = 0;
	if (!src)
		return (NULL);
	len = strvlen(src) + 2;
	dst = (char **)malloc((sizeof(char *)) * len);
	if (!dst)
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	while (src[i])
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			free_strv(dst);
			print_error_free(minishell, "Error\nMalloc failed.\n", 1);
		}
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

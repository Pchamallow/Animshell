/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:56:04 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/03 09:45:27 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** if index_of_occurence = 0
** search how many occurence we have in a string
** if index_of_occurence > 0
** search the index of the x occurence
** eg : index of second '/'
*/
int	strfind_occurences(char *src, char *target, int index_of_occurrence)
{
	int	occurrences;
	int	begin;
	int	len;
	int	i;

	i = 0;
	begin = 0;
	occurrences = 0;
	len = ft_strlen(target);
	while (src && src[i])
	{
		if (strfind(&src[i], target, 1) >= 0)
		{
			begin = i;
			i += len - 1;
			occurrences++;
			if (index_of_occurrence > 0 && occurrences == index_of_occurrence)
				return (begin);
		}
		i++;
	}
	if (occurrences)
		return (occurrences);
	return (0);
}

int	strfind_last(char *src, char *target)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	j = -1;
	len = ft_strlen(target);
	while (src && src[i])
	{
		if (strfind(&src[i], target, 1))
		{
			i += len;
			j = i;
		}
		i++;
	}
	if (j >= 0)
		return (j);
	return (0);
}

int	strfind(char *src, char *target, bool end)
{
	int	i;
	int	j;
	int	begin;

	i = 0;
	j = 0;
	begin = 0;
	while (src && src[i])
	{
		j = 0;
		if (src[i] == target[j])
		{
			begin = i;
			while (target[j] && target[j] == src[i + j])
				j++;
			if (!target[j])
			{
				if (!end)
					return (begin);
				return (i + j - 1);
			}
		}
		i++;
	}
	return (-1);
}

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

char	*join_oldnew(char *old, char *new, bool invert_order)
{
	char	*new_path;

	if (invert_order)
		new_path = ft_strjoin(old, new);
	else
		new_path = ft_strjoin(new, old);
	free(new);
	if (!new_path)
		return (NULL);
	return (new_path);
}

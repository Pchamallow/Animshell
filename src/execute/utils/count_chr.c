/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_chr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 14:25:11 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/09 17:20:16 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_chr(char *str, char c, bool followed)
{
	int i;
	int count;

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
	return (followed ? count : 0);
}

int	remove_begin(char **str, char c, int start, int end)
{
	char	*new;
	// int		end;

	// if ((*str)[0] == '.' && (*str)[1] == '/')
	// // mettre cette a part pour conserve la fonction modulable dans tous le projet
	// {
	// 	i += 2;
	// 	end = i;
	while ((*str)[start] && (*str)[start] == c)
	{
		start++;
		end++;
	}
	end--;
	if (end <= 0)
		return (0);
	new = ft_substr(*str, end, ft_strlen(*str) - end);
	if (!new)
		return (1);
	free(*str);
	*str = new;
	return (0);
}

int	remove_end(char **str, char c)
{
	char	*new;
	int		end;

	end = ft_strlen(*str) - 1;
	while (end > 0 && (*str)[end] && (*str)[end] == c)
		end--;
	end++;
	new = ft_substr(*str, 0, end);
	if (!new)
		return (1);
	free(*str);
	*str = new;
	return (0);
}

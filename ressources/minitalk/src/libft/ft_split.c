/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 16:14:25 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/04 23:22:00 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countwords(char const *s, char c)
{
	size_t	i;
	size_t	count;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static void	ft_freearray(char **s_array)
{
	size_t	i;

	i = 0;
	while (s_array[i] != NULL)
	{
		free(s_array[i]);
		i++;
	}
	free(s_array);
}

static char	*ft_fillarray(char const *s, size_t *j, char c, char *str)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
		(*j)++;
	}
	str[i] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	count;
	char	**s_array;

	i = 0;
	j = 0;
	count = ft_countwords(s, c);
	s_array = malloc(sizeof(char *) * (count + 1));
	if (!s_array)
		return (NULL);
	while (i < count)
	{
		while (s[j] && s[j] == c)
			j++;
		s_array[i] = ft_fillarray(s + j, &j, c, s_array[count]);
		if (s_array[i] == NULL)
		{
			ft_freearray(s_array);
			return (NULL);
		}
		i++;
	}
	s_array[i] = NULL;
	return (s_array);
}

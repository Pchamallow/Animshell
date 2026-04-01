/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:59:57 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 19:32:51 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(char **tab)
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static size_t	ft_sizeword(char *str, char c, size_t start)
{
	size_t	size;

	size = 1;
	while (str[start] != c && str[start] != '\0')
	{
		start++;
		size++;
	}
	return (size);
}

static int	ft_nbwords(char const *str, char c)
{
	size_t	i;
	size_t	flag;

	i = 0;
	flag = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
		{
			flag++;
			while (str[i] && str[i] != c)
				i++;
		}
	}
	return (flag);
}

static char	**ft_tab(char **tab, char *s, char c)
{
	size_t	sizeword;
	size_t	start;
	size_t	nbwords;
	size_t	nb;

	start = 0;
	nb = 0;
	nbwords = ft_nbwords(s, c);
	while (s[start] == c)
		start++;
	while (nb < nbwords)
	{
		sizeword = ft_sizeword(s, c, start);
		tab[nb] = ft_substr(s, start, sizeword - 1);
		if (!tab[nb++])
		{
			ft_free(tab);
			return (NULL);
		}
		start = start + sizeword;
		while (s[start] == c)
			start++;
	}
	tab[nb] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;

	if (!s)
		return (NULL);
	tab = ft_calloc(sizeof(char *), (ft_nbwords(s, c) + 1));
	if (!tab)
		return (NULL);
	tab = ft_tab(tab, (char *)s, c);
	return (tab);
}

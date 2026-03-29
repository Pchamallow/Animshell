/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:02:57 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 19:32:15 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	s;
	size_t	e;
	char	*tab;

	i = 0;
	s = 0;
	e = ft_strlen(s1) - 1;
	if (!s1 || !set || e > 2147483647)
	{
		tab = ft_substr("", 0, 1);
		return (tab);
	}
	while (s1[s] && ft_strchr(set, s1[s]))
		s++;
	while (e > s && ft_strchr(set, s1[e]))
		e--;
	tab = ft_substr(s1, s, (e - s + 1));
	if (tab == NULL)
		return (NULL);
	while (s <= e)
		tab[i++] = s1[s++];
	return (tab);
}

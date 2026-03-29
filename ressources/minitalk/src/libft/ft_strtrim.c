/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:35:43 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/02 22:20:08 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_findchr(char c, const char *set)
{
	size_t	i;

	i = 0;
	if (!set)
		return (0);
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_copystr(size_t start, size_t len, char *str, const char *s1)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		str[i] = s1[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	start;
	size_t	len;

	if (!s1)
		return (NULL);
	start = 0;
	len = ft_strlen(s1);
	while (ft_findchr(s1[start], set))
		start++;
	while (len > start && ft_findchr(s1[len - 1], set))
		len--;
	len = len - start;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str = ft_copystr(start, len, str, s1);
	return (str);
}

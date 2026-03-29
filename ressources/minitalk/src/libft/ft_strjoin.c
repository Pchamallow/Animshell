/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:40:37 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/05 10:45:00 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_len(char const *str)
{
	size_t	len;

	len = 0;
	if (str)
		len = ft_strlen(str);
	return (len);
}

static size_t	ft_copystr(char const *src, char *dest)
{
	size_t	i;

	i = 0;
	while (src && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	total_len;

	if (!s1 && !s2)
		return (NULL);
	i = 0;
	total_len = ft_len(s1) + ft_len(s2);
	str = malloc(sizeof(char) * (total_len + 1));
	if (!str)
		return (NULL);
	i = ft_copystr(s1, str);
	i = i + ft_copystr(s2, str + i);
	str[i] = '\0';
	return (str);
}

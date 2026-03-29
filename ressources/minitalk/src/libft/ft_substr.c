/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:27:35 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/02 22:18:41 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	str_len;

	if (!s)
		return (NULL);
	i = 0;
	if (ft_strlen(s) > start)
		str_len = ft_strlen(s) - start;
	else
		str_len = 0;
	if (str_len > len)
		str_len = len;
	str = malloc(sizeof(char) * (str_len + 1));
	if (!str)
		return (NULL);
	while (i < str_len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

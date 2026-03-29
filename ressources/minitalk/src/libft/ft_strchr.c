/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:49:35 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/01 16:46:01 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	size_t			s_length;
	unsigned char	to_find;

	i = 0;
	s_length = ft_strlen(s);
	to_find = (unsigned char)c;
	while (i <= s_length)
	{
		if (s[i] == to_find)
			return ((char *)s + i);
		i++;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:28:16 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/04 23:37:50 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	long long		i;
	unsigned char	to_find;

	i = (long long)ft_strlen(s);
	to_find = (unsigned char)c;
	while (i >= 0)
	{
		if (s[i] == to_find)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}

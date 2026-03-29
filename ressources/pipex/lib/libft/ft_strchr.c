/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 14:41:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 19:32:43 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	int		j;
	char	*a;
	char	to_find;

	i = 0;
	j = ft_strlen(s);
	a = (char *)s;
	to_find = (char) c;
	while (i <= j)
	{
		if (a[i] == to_find)
			return (&a[i]);
		i++;
	}
	return (NULL);
}

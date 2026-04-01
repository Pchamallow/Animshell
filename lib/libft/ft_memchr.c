/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 19:38:43 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 19:33:50 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*src;
	unsigned char	to_find;

	i = 0;
	src = (unsigned char *) s;
	to_find = (unsigned char) c;
	while (i < n)
	{
		if (src[i] == to_find)
			return (&src[i]);
		i++;
	}
	return (NULL);
}

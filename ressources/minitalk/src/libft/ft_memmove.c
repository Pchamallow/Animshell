/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 14:37:29 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/05 10:29:04 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	long long		i;
	unsigned char	*str_src;
	unsigned char	*str_dest;

	i = (long long)n - 1;
	str_src = (unsigned char *)src;
	str_dest = (unsigned char *)dest;
	if (str_dest < str_src)
		str_dest = ft_memcpy(str_dest, str_src, n);
	else
	{
		while (i >= 0)
		{
			str_dest[i] = str_src[i];
			i--;
		}
	}
	return (dest);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:42:31 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/01 00:14:54 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_addstr(char *dst, const char *src, size_t max, size_t i)
{
	size_t	j;

	j = 0;
	while (src[j] && j < max)
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	dst[i] = '\0';
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	result;
	size_t	max;

	i = 0;
	if (size == 0)
	{
		result = ft_strlen(src);
		return (result);
	}
	max = size - ft_strlen(dst) - 1;
	result = ft_strlen(dst);
	if (result > size)
		result = ft_strlen(src) + size;
	else
		result = ft_strlen(src) + result;
	while (dst[i])
		i++;
	if (i < size)
		ft_addstr(dst, src, max, i);
	return (result);
}

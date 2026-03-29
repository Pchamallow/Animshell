/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:03:15 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/25 21:35:06 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*src;

	i = 0;
	j = 0;
	if (!big)
		return (NULL);
	src = (char *)big;
	if (little[i] == 0 || !little)
		return (src);
	while (src[i] && (i + j) < len)
	{
		if (src[i + j] == little[j])
			j++;
		else
		{
			j = 0;
			i++;
		}
		if (!little[j])
			return (&src[i]);
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:08:19 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/07 16:38:12 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		i;
	size_t		j;
	size_t		len_s;
	char		*sb;

	i = (size_t) start;
	j = 0;
	len_s = ft_strlen(s);
	if (start >= len_s)
	{
		sb = (char *)ft_calloc(sizeof(char), 1);
		return (sb);
	}
	sb = ft_calloc(sizeof(char), (len + 1));
	if (sb == NULL)
		return (NULL);
	while (s[i] && i < (start + len))
		sb[j++] = s[i++];
	sb[j] = '\0';
	return (sb);
}

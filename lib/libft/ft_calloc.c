/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 12:55:59 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/12 14:58:58 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*tab;

	if ((nmemb * size) > ((size_t)-1))
		return (NULL);
	tab = (char *)malloc(size * nmemb);
	if (tab == NULL)
		return (NULL);
	ft_bzero(tab, (nmemb * size));
	return (tab);
}

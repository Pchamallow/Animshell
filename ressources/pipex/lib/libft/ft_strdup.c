/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:35:18 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/12 15:00:01 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*create_tab;
	int		i;

	i = 0;
	create_tab = (char *)malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (create_tab == NULL)
		return (NULL);
	while (src[i] != '\0')
	{
		create_tab[i] = src[i];
		i++;
	}
	create_tab[i] = '\0';
	return (create_tab);
}

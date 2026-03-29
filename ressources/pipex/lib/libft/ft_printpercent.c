/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printpercent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:28:01 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 18:49:18 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_percenterror(const char *format, va_list args)
{
	if (ft_strlen(format) == 1)
	{
		va_end (args);
		return (-1);
	}
	return (0);
}

int	ft_printpercent(va_list args, int fd)
{
	char	c;

	(void)args;
	c = '%';
	write(fd, &c, 1);
	return (1);
}

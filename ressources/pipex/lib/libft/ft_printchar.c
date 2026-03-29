/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printchar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 18:21:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 18:48:27 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printchar(va_list args, int fd)
{
	ft_putchar_fd(va_arg(args, int), fd);
	return (1);
}

int	ft_printnbr(va_list args, int fd)
{
	int		count;

	count = ft_putnbr_fd(va_arg(args, int), fd);
	return (count);
}

int	ft_printunbr(va_list args, int fd)
{
	int	count;

	count = ft_putunbr_fd(va_arg(args, int), fd);
	return (count);
}

int	ft_printstr(va_list args, int fd)
{
	int		count;
	char	*str;

	str = va_arg(args, char *);
	if (!str)
	{
		write(fd, "(null)", 6);
		return (6);
	}
	count = ft_putstr_fd(str, fd);
	return (count);
}

int	ft_line(char format, int fd)
{
	if (format == '\n')
	{
		ft_putchar_fd('\n', fd);
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 14:45:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 18:49:37 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printpointer(va_list args, int fd)
{
	int			count;
	size_t		number;

	count = 0;
	number = (size_t)va_arg(args, void *);
	if (number == 0)
	{
		write(fd, "(nil)", 5);
		return (5);
	}
	write(fd, "0x", 2);
	count += 2;
	count += ft_putnbr_st_fd(number, 16, "0123456789abcdef", fd);
	return (count);
}

int	ft_printhexalow(va_list args, int fd)
{
	return (ft_putnbr_ui_fd(va_arg(args, unsigned int),
			16, "0123456789abcdef", fd));
}

int	ft_printhexaup(va_list args, int fd)
{
	return (ft_putnbr_ui_fd(va_arg(args, unsigned int),
			16, "0123456789ABCDEF", fd));
}

int	ft_putnbr_st_fd(size_t n, size_t baseformat, char *base, int fd)
{
	int	count;

	count = 0;
	if (n >= baseformat)
		count += ft_putnbr_st_fd(n / baseformat, baseformat, base, fd);
	count += ft_putchar_fd(base [n % baseformat], fd);
	return (count);
}

int	ft_putnbr_ui_fd(unsigned int n, unsigned int baseformat,
					char *base, int fd)
{
	int	count;

	count = 0;
	if (n >= baseformat)
		count += ft_putnbr_ui_fd(n / baseformat, baseformat, base, fd);
	count += ft_putchar_fd(base [n % baseformat], fd);
	return (count);
}

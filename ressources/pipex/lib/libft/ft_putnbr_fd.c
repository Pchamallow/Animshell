/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 09:16:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/01/31 18:48:13 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	int		count;
	char	c;

	count = 0;
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = n * -1;
		count++;
	}
	if (n / 10)
		count += ft_putnbr_fd(n / 10, 1);
	c = n % 10 + '0';
	write(fd, &c, 1);
	count++;
	return (count);
}

int	ft_putunbr_fd(long int n, int fd)
{
	int		count;
	char	c;

	count = 0;
	if (n < 0)
	{
		n = (UINT_MAX) + n + 1;
		if (n < 0)
			n = n * -1;
	}
	if (n / 10)
		count += ft_putnbr_fd(n / 10, 1);
	c = n % 10 + '0';
	write(fd, &c, 1);
	count++;
	return (count);
}

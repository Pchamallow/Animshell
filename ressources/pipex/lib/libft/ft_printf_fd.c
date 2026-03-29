/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 11:45:50 by pswirgie          #+#    #+#             */
/*   Updated: 2026/02/15 17:25:49 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_initformat(int (**ft_format)(va_list args, int fd))
{
	int	i;

	i = 127;
	while (i < 0)
	{
		i--;
		ft_format[i] = 0;
	}
	ft_format['c'] = ft_printchar;
	ft_format['d'] = ft_printnbr;
	ft_format['i'] = ft_printnbr;
	ft_format['p'] = ft_printpointer;
	ft_format['s'] = ft_printstr;
	ft_format['u'] = ft_printunbr;
	ft_format['x'] = ft_printhexalow;
	ft_format['X'] = ft_printhexaup;
	ft_format['%'] = ft_printpercent;
	return ;
}

static int	helper(const char **format, va_list args)
{
	if (ft_percenterror(*format, args) == -1)
		return (-1);
	if (ft_strlen(*format) == 1)
	{
		va_end (args);
		return (-1);
	}
	(*format)++;
	return (0);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	int		count;
	va_list	args;
	int		(*ft_format[127])(va_list, int);

	count = 0;
	if (fd < 0 || !format)
		return (-1);
	va_start(args, format);
	ft_initformat(ft_format);
	while (*format)
	{
		if (*format == '%')
		{
			if (helper(&format, args) == -1)
				return (-1);
			count += ft_format[(int)*format](args, fd);
		}
		else
			count += ft_putchar_fd(*format, fd);
		format++;
	}
	va_end (args);
	return (count);
}

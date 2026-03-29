/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:14:40 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/22 16:12:50 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_write_error(void)
{
	char	*str;

	str = ft_strdup("Format error\n");
	if (!str)
		return ;
	ft_putstr_fd(str, 2);
	free(str);
}

static int	ft_check(const char *format)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (ft_strchr(VAR_LIST, format[i + 1]) != NULL)
				i++;
			else
				return (-1);
			if (format[i] != '%')
				count++;
		}
		i++;
	}
	return (count);
}

static int	ft_write_txt(const char *format, int *i, int *count)
{
	int		len;
	char	*str;

	len = 0;
	while (format[len] && format[len] != '%')
		len++;
	str = ft_substr(format, 0, len);
	if (!str)
		return (0);
	ft_putstr_fd(str, 1);
	free(str);
	(*i) += len;
	(*count) += len;
	return (1);
}

static void	ft_write_var(const char var, va_list param, int *i, int *count)
{
	if (var == 'c')
		ft_var_c((char)va_arg(param, int), count);
	else if (var == 's')
		ft_var_s(va_arg(param, char *), count);
	else if (var == 'd' || var == 'i')
		ft_var_dori(va_arg(param, int), count);
	else if (var == 'u')
		ft_var_u(va_arg(param, unsigned int), count);
	else if (var == 'p')
		ft_var_p(va_arg(param, unsigned long), count);
	else if (var == 'x')
		ft_var_x(va_arg(param, unsigned int), count);
	else if (var == 'X')
		ft_var_upperx(va_arg(param, unsigned int), count);
	else if (var == '%')
		ft_var_c('%', count);
	(*i) += 2;
}

int	ft_printf(const char *format, ...)
{
	va_list	param;
	int		i;
	int		count;

	if (!format)
		return (-1);
	va_start(param, format);
	if (ft_check(format) < 0)
	{
		ft_write_error();
		va_end(param);
		return (-1);
	}
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] != '%')
			ft_write_txt(format + i, &i, &count);
		else
			ft_write_var(format[i + 1], param, &i, &count);
	}
	va_end(param);
	return (count);
}

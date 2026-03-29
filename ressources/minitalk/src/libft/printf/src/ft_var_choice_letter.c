/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_var_choice_letter.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:30:18 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/22 17:11:32 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	ft_write_null(int *count, char c)
{
	if (c == 'p')
	{
		write(1, "(nil)", 5);
		(*count) += 5;
	}
	else if (c == 's')
	{
		write(1, "(null)", 6);
		(*count) += 6;
	}
	else if (c == 'm')
	{
		write(1, "(malloc problem)", 16);
		(*count) += 16;
	}
}

void	ft_var_c(char c, int *count)
{
	ft_putchar_fd(c, 1);
	(*count)++;
}

void	ft_var_s(char *s, int *count)
{
	if (!s)
	{
		ft_write_null(count, 's');
		return ;
	}
	ft_putstr_fd(s, 1);
	(*count) += (int)ft_strlen(s);
}

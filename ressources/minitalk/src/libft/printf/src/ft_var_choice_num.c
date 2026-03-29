/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_var_choice_num.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:34:37 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/22 17:12:46 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	ft_var_dori(int d, int *count)
{
	char	*s;

	s = ft_itoa(d);
	if (!s)
	{
		ft_write_null(count, 'm');
		return ;
	}
	ft_putstr_fd(s, 1);
	(*count) += (int)ft_strlen(s);
	free(s);
}

void	ft_var_u(unsigned int u, int *count)
{
	char	*s;

	s = ft_utoa((unsigned long)u);
	if (!s)
	{
		ft_write_null(count, 'm');
		return ;
	}
	ft_putstr_fd(s, 1);
	(*count) += (int)ft_strlen(s);
	free(s);
}

void	ft_var_p(unsigned long p, int *count)
{
	char	*s;
	char	*s_address;

	if (!p)
	{
		ft_write_null(count, 'p');
		return ;
	}
	s = ft_utoa_base(p);
	if (!s)
	{
		ft_write_null(count, 'm');
		return ;
	}
	s_address = ft_strjoin("0x", s);
	if (!s_address)
	{
		free(s);
		ft_write_null(count, 'm');
		return ;
	}
	ft_putstr_fd(s_address, 1);
	(*count) += (int)ft_strlen(s_address);
	free(s_address);
	free(s);
}

void	ft_var_x(unsigned int u, int *count)
{
	char	*s;

	s = ft_utoa_base((unsigned long)u);
	if (!s)
	{
		ft_write_null(count, 'm');
		return ;
	}
	ft_putstr_fd(s, 1);
	(*count) += (int)ft_strlen(s);
	free(s);
}

void	ft_var_upperx(unsigned int u, int *count)
{
	char	*s;

	s = ft_utoa_base((unsigned long)u);
	if (!s)
	{
		ft_write_null(count, 's');
		return ;
	}
	s = ft_str_toupper(s);
	ft_putstr_fd(s, 1);
	(*count) += (int)ft_strlen(s);
	free(s);
}

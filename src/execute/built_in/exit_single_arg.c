/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_single_arg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 16:12:53 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/24 16:14:06 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long	error_overflow(long nb)
{
	if (nb > 255)
		nb = nb % 256;
	return (nb);
}

static bool	convert_to_ll(const char *str, long long *result, int sign, int i)
{
	int			digit;
	long long	neg_result;

	digit = str[i] - '0';
	if (sign == 1 && *result > (LLONG_MAX - digit) / 10)
		return (false);
	else if (sign == -1)
	{
		neg_result = (*result) * -1;
		if (neg_result < (LLONG_MIN + digit) / 10)
			return (false);
	}
	*result = (*result) * 10 + digit;
	return (true);
}

static int	get_sign(char c, int *i)
{
	if (c == '+' || c == '-')
	{
		(*i)++;
		if (c == '-')
			return (-1);
	}
	return (1);
}

static void	ft_atoll_exit(const char *str, long long *out)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	i = 0;
	while (str[i] && is_whitespace(str[i]))
		i++;
	sign = get_sign(str[i], &i);
	while (str[i] && ft_isdigit((unsigned char)str[i]))
	{
		if (!convert_to_ll(str, &result, sign, i))
		{
			error_cmd_args("exit", (char *)str, "numeric argument required");
			*out = 2;
			return ;
		}
		i++;
	}
	*out = result * sign;
	return ;
}

int	exit_single_arg(t_minishell *minishell, char *nb)
{
	if (is_num_single_sign(nb) != 0)
	{
		ft_atoll_exit(nb, &minishell->exec.error);
		minishell->exec.error = error_overflow(minishell->exec.error);
		return (0);
	}
	else if (!ft_strcmp(nb, "--"))
		return (0);
	else
		return (1);
}

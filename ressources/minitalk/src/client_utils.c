/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 19:21:50 by stkloutz          #+#    #+#             */
/*   Updated: 2026/03/05 23:18:23 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

static long	ft_atol(const char *nptr)
{
	int		i;
	int		sign;
	long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		nb *= 10;
		nb = nb + (nptr[i] - '0');
		i++;
	}
	nb = nb * sign;
	return (nb);
}

static bool	is_valid_pid(char *str, int *server_pid)
{
	long	nb;
	int		i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
		return (false);
	if (str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (false);
	while (str[i] && (ft_isdigit(str[i])))
		i++;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] != '\0')
		return (false);
	nb = ft_atol(str);
	if (nb <= 0 || nb > INT_MAX)
		return (false);
	*server_pid = (int)nb;
	return (true);
}

bool	check_input(int ac, char **av, int *server_pid)
{
	if (ac != 3)
	{
		ft_putendl_fd(ERR_NB_ARG SYNTAX USAGE, 2);
		return (false);
	}
	if (!is_valid_pid(av[1], server_pid))
	{
		ft_putendl_fd(ERR_INT_LIMIT, 2);
		return (false);
	}
	return (true);
}

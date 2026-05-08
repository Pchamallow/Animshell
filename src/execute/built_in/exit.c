/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 14:26:02 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 16:48:32 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	error_overflow(long nb)
{
	if (nb > 255)
		nb = nb % 256;
	return (nb);
}

/*
** return
**	0	= not a only number or sign
**	1	= number
**	2	= +number
**	-1	= -number
*/
int	is_num_single_sign(char *str)
{
	int	i;
	int	sign;
	int	doubles;

	i = 0;
	doubles = 0;
	sign = 1;
	if (str[0] == '-' && (str[1] >= '0' && str[1] <= '9'))
		sign = -1;
	if (str[0] == '+' && (str[1] >= '0' && str[1] <= '9'))
		sign = 2;
	while (str[i]
			&& ((str[i] >= '0' && str[i] <= '9')
			|| str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-' || str[i] == '+')
			doubles++;
		i++;
	}
	if (str[i] || doubles >= 2
		|| !(str[i - 1] >= '0' && str[i - 1] <= '9'))
		return (0);
	return (sign);
}

int	exit_single_arg(t_minishell *minishell, char *nb)
{
	if (is_num_single_sign(nb) != 0)
	{
		minishell->exec.error = error_overflow(ft_atol(nb));
		return (0);
	}
	else
		return (1);
}

int	exit_multiple_args(t_minishell *minishell, char *nb)
{
	if (is_num_single_sign(nb) != 0)
	{
		error_cmd_args("exit", nb, "too many arguments");
		minishell->exec.error = 1;
		return (0);// exit false
	}
	else
		return (1);
}

int	exit_gestion_args(t_minishell *minishell, char *nb)
{
	int		error_numeric;
	int		args;

	error_numeric = 0;
	args = nb_args(minishell->token);
	if (args == 1 && exit_single_arg(minishell, nb))
		error_numeric++;
	else if (args > 1)
	{
		if (exit_multiple_args(minishell, nb) == 0)
			return (1);
		else
			error_numeric++;
	}
	if (error_numeric)
	{
		error_cmd_args("exit", nb, "numeric argument required");
		minishell->exec.error = 2;
	}
	return (0);
}

void	is_exit(t_minishell *minishell)
{
	if (ft_strcmp(minishell->token->value, "exit") == 0
		&& !nb_pipes(minishell->token))
	{
		ft_printf_fd(1, "exit\n");
		if (minishell->token->next)
		{
			if (exit_gestion_args(minishell, minishell->token->next->value) == 0)
			{
				free_all(minishell);
				exit(minishell->exec.error);
			}
		}
		else
		{
			free_all(minishell);
			exit(minishell->exec.error);
		}
	}
	else if (ft_strcmp(minishell->token->value, "exit") == 0
		&& nb_pipes(minishell->token))
		minishell->exec.error = 2;
}

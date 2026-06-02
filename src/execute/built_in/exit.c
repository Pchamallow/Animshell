/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 14:26:02 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 17:06:23 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** return
**	0	= not a only number or sign
**	1	= number
**	-1	= -number
*/
int	is_num_single_sign(char *str)
{
	int	i;
	int	number;

	i = 0;
	number = 0;
	while (str[i] && is_whitespace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && (ft_isdigit(str[i]) || is_whitespace(str[i])))
	{
		if (ft_isdigit(str[i]) && (((str[i + 1] && !ft_isdigit(str[i + 1])))
				|| !str[i + 1]))
			number++;
		i++;
	}
	if (str[i] || number != 1)
		return (0);
	if (number == 1)
		return (1);
	return (0);
}

int	exit_multiple_args(t_minishell *minishell, char *nb)
{
	if (is_num_single_sign(nb) != 0)
	{
		error_cmd_args("exit", nb, "too many arguments");
		minishell->exec.error = 1;
		return (0);
	}
	else
		return (1);
}

static int	exit_gestion_args(t_minishell *minishell, t_pipe *pipe, char *nb)
{
	int		error_numeric;
	int		args;

	error_numeric = 0;
	args = pipe->nb_args;
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

void	is_exit(t_minishell *minishell, t_pipe *pipe)
{
	bool	is_arg;

	is_arg = false;
	if (pipe->builtin_kind != EXIT)
		return ;
	if (minishell->exec.nb_pipes && !pipe->cmd->next)
	{
		minishell->exec.error = minishell->exec.error_last_prompt;
		return ;
	}
	if (!minishell->exec.nb_pipes)
		ft_printf_fd(1, "exit\n");
	if (pipe->cmd->next)
	{
		is_arg = true;
		if (exit_gestion_args(minishell, pipe, pipe->cmd->next->value))
			return ;
	}
	if (!minishell->exec.nb_pipes)
	{
		free_all(minishell);
		if (!minishell->exec.nb_pipes && !is_arg)
			exit(minishell->exec.error_last_child);
		exit(minishell->exec.error);
	}
}

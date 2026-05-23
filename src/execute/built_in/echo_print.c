/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:13:49 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/23 18:14:19 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_print_arg(t_token *args, bool *is_arg)
{
	if (*is_arg == true)
		ft_printf_fd(1, " ");
	if (!args->value)
		ft_printf_fd(1, "no arg value\n");
	ft_printf_fd(1, "%s", args->value);
	*is_arg = true;
}

/* ECHO_PRINT ************
** - prints a given string
** - only if no "-n" option
** - if no string : prints '/n'
********************** */
int	echo_print(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*args;
	bool	is_arg;

	args = NULL;
	is_arg = false;
	if (minishell->builtin.echo.for_prompt == true)
		return (0);
	if (pipe->cmd->next && pipe->cmd->next->type != PIPE)
	{
		args = pipe->cmd->next;
		while (args)
		{
			if (args->type == PIPE)
			{
				if (is_arg == true)
					ft_printf_fd(1, "\n");
				return (0);
			}
			if (args->type == IS_ARG)
				echo_print_arg(args, &is_arg);
			args = args->next;
		}
	}
	ft_printf_fd(1, "\n");
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:13:49 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/24 18:05:05 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** options	=	-n	-nnnnnnnnn 
** not		=	-	n
*/
int	echo_is_option(char *str)
{
	bool	option;
	int		i;

	i = 0;
	option = false;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
		{
			option = true;
			i++;
		}
	}
	if (!str[i] && option == true)
		return (1);
	return (0);
}

static void	echo_print_arg(t_token *args, bool *is_arg)
{
	if (*is_arg == true)
		ft_printf_fd(1, " ");
	if (!args->value)
		ft_printf_fd(1, "minishell : no arg value\n");
	ft_printf_fd(1, "%s", args->value);
	*is_arg = true;
}

static int	echo_args(t_token *args, bool *is_arg)
{
	while (args)
	{
		if (args->type == PIPE)
		{
			if (*is_arg == true)
				ft_printf_fd(1, "\n");
			return (0);
		}
		else if (args->type == IS_ARG)
			echo_print_arg(args, is_arg);
		args = args->next;
	}
	return (1);
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
		if (echo_is_option(args->value) && !args->next)
			return (0);
		if (!echo_args(args, &is_arg))
			return (0);
	}
	ft_printf_fd(1, "\n");
	return (0);
}

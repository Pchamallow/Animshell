/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:13:49 by stkloutz          #+#    #+#             */
/*   Updated: 2026/06/02 12:06:55 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** options	=	-n	-nnnnnnnnn 
** not		=	-	n
*/
static int	echo_is_option(char *str)
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

static void	echo_args(t_token *args, bool is_option)
{
	bool	is_arg;

	is_arg = false;
	while (args && str_iswhitespaces(args->value))
		args = args->next;
	while (args && args->type != PIPE)
	{
		if (args->type == IS_ARG)
			echo_print_arg(args, &is_arg);
		args = args->next;
	}
	if (is_option == false)
		ft_printf_fd(1, "\n");
	return ;
}

/* ECHO_PRINT *********************
** - prints a given string
** - options 
**		----- + n = print on new line
**		- + nnnnnnn = print without \n
**		----- + nnnnnn = print on new line
** - if no options : prints '/n'
**********************************/
void	echo(t_pipe *pipe)
{
	t_token	*args;
	bool	is_option;

	is_option = false;
	if (pipe->cmd->next)
	{
		args = pipe->cmd->next;
		while (args && echo_is_option(args->value))
		{
			is_option = true;
			args = args->next;
		}
		if (args)
			echo_args(args, is_option);
	}
}

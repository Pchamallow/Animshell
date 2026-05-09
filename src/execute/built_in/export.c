/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:11:33 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/09 17:37:37 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_unique(t_token *arg)
{
	t_token	*first_arg;

	first_arg = arg;
	arg = arg->next;
	while (arg)
	{
		if (is_same_name(first_arg->value, arg))
			return (false);
		arg = arg->next;
	}
	return (true);
}

int	count_lines_to_add(t_minishell *minishell, t_token *arg,
		bool *valid_var_to_add)
{
	int	count;

	count = 0;
	while (arg && arg->type != PIPE)
	{
		if (arg->type == IS_ARG && !is_valid_arg(arg->value))
		{
			ft_printf_fd(2, "minishell: export: \'%s\':", arg->value);
			ft_printf_fd(2, " not a valid identifier\n");
			minishell->exec.error = 1;
		}
		else if (arg->type == IS_ARG && is_valid_arg(arg->value))
		{
			*valid_var_to_add = true;
			if (found_var(minishell->exec.envp, arg->value) < 0
				&& is_unique(arg))
				count++;
		}
		arg = arg->next;
	}
	return (count);
}

bool	export_option_error(t_minishell *minishell, t_token *arg)
{
	if (arg->type == IS_ARG && arg->value[0] == '-' && arg->value[1] != '\0')
	{
		ft_printf_fd(2, "minishell: export: -%c", arg->value[1]);
		ft_printf_fd(2, ": invalid option\n");
		ft_printf_fd(2, "export: usage: export [nom[=valeur] ...]\n");
		minishell->exec.error = 2;
		return (true);
	}
	return (false);
}

/*
** EXPORT WITH ARGUMENTS ************************
** - no options, as requested by the subject
** - adds valid variables in char **envp
** ! if export is called without arguments :
**		-> see export_print function
**	*********************************************
*/
int	export(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*arg;
	int		count;
	char	**new_envp;
	bool	valid_var_to_add;

	minishell->exec.error = 0;
	if (pipe->nb_args == 0)
		return (minishell->exec.error);
	arg = pipe->cmd->next;
	if (export_option_error(minishell, arg))
		return (minishell->exec.error);
	valid_var_to_add = false;
	count = count_lines_to_add(minishell, arg, &valid_var_to_add);
	if (pipe->next)
		return (minishell->exec.error);
	if (valid_var_to_add)
	{
		new_envp = update_envp(minishell, arg, count);
		free(minishell->exec.envp);
		minishell->exec.envp = new_envp;
	}
	return (minishell->exec.error);
}

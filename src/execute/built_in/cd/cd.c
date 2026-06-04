/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:58:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/04 13:25:15 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_errors_args(t_minishell *minishell, t_pipe *pipe)
{
	if (pipe->cmd->cmd_args && pipe->cmd->cmd_args[1])
	{
		error_cmd_args("cd", NULL, "too many arguments");
		minishell->exec.error = 1;
		return (1);
	}
	return (0);
}

static int	cd_get_args(t_minishell *minishell, t_pipe *pipe)
{
	if (!pipe->cmd->cmd_args || !pipe->cmd->cmd_args[0])
	{
		minishell->builtin.cd.result = root();
		if (!minishell->builtin.cd.result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return (0);
	}
	if (cd_errors_args(minishell, pipe))
		return (1);
	if (pipe->cmd->cmd_args[0])
	{
		if (!ft_strcmp(pipe->cmd->cmd_args[0], "-"))
		{
			pwd_update(minishell);
			return (1);
		}
		minishell->builtin.cd.result = ft_strdup(pipe->cmd->cmd_args[0]);
		if (!minishell->builtin.cd.result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		if (minishell->builtin.cd.result[0] == '~')
			root_with_folder(minishell);
		if (!ft_strcmp(minishell->builtin.cd.result, "./"))
			return (0);
	}
	return (0);
}

static void	free_and_set_null(t_minishell *minishell)
{
	free(minishell->builtin.cd.result);
	minishell->builtin.cd.result = NULL;
}

static void	print_error_cd(t_minishell *minishell)
{
	ft_printf_fd(2, "minishell: cd: ");
	perror(minishell->builtin.cd.result);
	minishell->exec.error = 1;
}

/*
** CD *********************************************
** Move the current position in a directory
** specified.
**
** - only a relative or absolute path, like required
** by the subject
**
** - no argument = or > 1 = error message
**
** - if no directory permissions
**		-> remove directory from path
**
** - if argument is valid
** 		- we move in folder
**		- replace pwd
**			- path is absolute = /newpath
**				-> copy it
**			- path is relative 
**				= ./newpath
**				-> concatenate with the old path
**				= ../newpath
**				-> replace last folder of old path
**					by new path
** 
*/
int	cd(t_minishell *minishell, t_pipe *pipe)
{
	int	error;

	error = 0;
	if (minishell->builtin.cd.result)
		free_and_set_null(minishell);
	if (cd_get_args(minishell, pipe))
		return (0);
	is_perm_folder(minishell, &minishell->builtin.cd);
	error = chdir(minishell->builtin.cd.result);
	if (error != 0)
		print_error_cd(minishell);
	if (minishell->exec.nb_pipes)
	{
		chdir(&minishell->builtin.pwd.result[4]);
		return (0);
	}
	if (error == 0)
	{
		replace_oldpwd(minishell, pipe);
		modify_pwd_in_envp(minishell);
	}
	if (is_pwd_invalid())
		error_getcwd(minishell, pipe);
	return (0);
}

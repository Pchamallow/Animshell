/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:58:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/14 10:36:54 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	root_from_pwd(char *pwd)
{
	int	slash;
	int	i;

	i = 0;
	slash = 0;
	while (pwd[i] && slash < 3)
	{
		if (pwd[i] == '/')
			slash++;
		i++;
	}
	return (i);
}

int	is_root(t_minishell *minishell)
{
	int	result;
	int	len;

	result = cpy_strvindex(&minishell->builtin.cd.result, minishell->exec.envp, "HOME=");
	if (result == 1)
		return (1);
	else if (result == -1)
	{
		len = root_from_pwd(minishell->builtin.pwd.result);
		minishell->builtin.cd.result = 
			ft_substr(minishell->builtin.pwd.result,
				4, len - 4);
		if (!minishell->builtin.cd.result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	return (0);
}

void	replace_oldpwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*path_pwd;
	int		result;

	path_pwd = NULL;
	if (pipe->cmd->cmd_args && pipe->cmd->cmd_args[0] && !ft_strcmp(pipe->cmd->cmd_args[0], "."))
		init_pwd(minishell);
	path_pwd = ft_substr(minishell->builtin.pwd.result, 4, ft_strlen(minishell->builtin.pwd.result) - 4);
	if (!path_pwd)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	result = strv_searchindex(minishell->exec.envp, "OLDPWD=");
	if (result == -1)
	{
		free(path_pwd);
		return ;
	}
	free(minishell->exec.envp[result]);
	minishell->exec.envp[result] = ft_strjoin("OLDPWD=", path_pwd);
	if (!minishell->exec.envp[result])
	{
		free(path_pwd);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(path_pwd);
}

void	root_with_folder(t_minishell *minishell)
{
	char	*home;
	char	*folder;
	int		result;

	result = cpy_strvindex(&home, minishell->exec.envp, "HOME=");
	if (result == 1)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	else if (result == -1)
		return ;
	folder = ft_substr(minishell->builtin.cd.result , 1, ft_strlen(minishell->builtin.cd.result ) - 1);
	if (!folder)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	free(minishell->builtin.cd.result);
	minishell->builtin.cd.result = ft_strjoin(home, folder);
	free(folder);
	free(home);
}

int	cd_errors_args(t_minishell *minishell, t_pipe *pipe)
{
	(void)pipe;
	if (pipe->cmd->cmd_args && pipe->cmd->cmd_args[1])
	{
		error_cmd_args("cd", NULL, "too many arguments");
		minishell->exec.error = 1;
		return (1);
	}
	return (0);
}

int cd_get_args(t_minishell *minishell, t_pipe *pipe)
{
	if (!pipe->cmd->cmd_args || !pipe->cmd->cmd_args[0])
	{
		if (is_root(minishell) == 1)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return (0);
	}
	if (cd_errors_args(minishell, pipe))
		return (1);
	if (pipe->cmd->cmd_args[0])
	{
		if (!ft_strcmp(pipe->cmd->cmd_args[0], "-"))
		{
			pwd(minishell, pipe);
			return (1);
		}
		minishell->builtin.cd.result = ft_strdup(pipe->cmd->cmd_args[0]);
		if (minishell->builtin.cd.result[0] == '~')
			root_with_folder(minishell);
		if (!ft_strcmp(minishell->builtin.cd.result, "./"))
			return (0);
	}
	return (0);
}

/*
** if PWD unset, return -> not create variable PWD
** else search index PWD in envp, free and cpy pwd
*/
void	modify_pwd_in_envp(t_minishell *minishell)
{
	int i;
	if (strv_searchindex(minishell->exec.envp, "PWD=") == -1)
		return ;
	init_pwd(minishell);
	i = strv_searchindex(minishell->exec.envp, "PWD=");
	if (i != -1)
	{
		free(minishell->exec.envp[i]);
		minishell->exec.envp[i] = ft_strdup(minishell->builtin.pwd.result);
	}
}

int	cd_move_dir(t_minishell *minishell)
{
	if (chdir(minishell->builtin.cd.result) != 0)
	{
		ft_printf_fd(2, "minishell: cd: ");
		perror(minishell->builtin.cd.result);
		minishell->exec.error = 1;
	}
	if (minishell->exec.nb_pipes)
	{
		chdir(&minishell->builtin.pwd.result[4]);
		return (-1);
	}
	return (0);
}

/*
** CD *********************************************
** Move the current position in a directory
** specified.
**
** - only a relative or absolute path, like required
** by the subject
**
** - no argument or > 1= error message
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
	if (minishell->builtin.cd.result)
	{
		free(minishell->builtin.cd.result);
		minishell->builtin.cd.result = NULL;
	}
	if (cd_get_args(minishell, pipe))
		return (0);
	if (cd_move_dir(minishell) == 0)
	{
		replace_oldpwd(minishell, pipe);
		modify_pwd_in_envp(minishell);
	}
	return (0);
}

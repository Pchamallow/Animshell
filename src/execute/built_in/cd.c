/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:58:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 14:25:43 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	path_kind(t_minishell *minishell, char **path)
{
	char	*new_path;

	if ((*path)[0] == '.' && (*path)[1] == '.' && (*path)[2] == '/')
		minishell->builtin.cd.path = RELATIVE_DOUBLE;
	else if ((*path)[0] == '.' && (*path)[1] == '/')
		minishell->builtin.cd.path = RELATIVE_SINGLE;
	else if ((*path)[0] == '/')
		minishell->builtin.cd.path = ABSOLUTE;
	else if ((*path)[0] == '.' && !(*path)[1])
		minishell->builtin.cd.path = STAY;
	else if (ft_isalpha((*path)[0]))
	{
		new_path = ft_strjoin("./", *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(*path);
		*path = new_path;
		minishell->builtin.cd.path = RELATIVE_SINGLE;
	}
}

static void	replace_pwd(t_minishell *minishell, char **path)
{
	int		index_pwd;

	index_pwd = strv_searchindex(minishell->exec.envp, "PWD=");
	path_clean(minishell, path, index_pwd);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	minishell->builtin.pwd.result = ft_strdup(*path);
	if (!minishell->builtin.pwd.result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	if (index_pwd != -1)
	{
		free(minishell->exec.envp[index_pwd]);
		minishell->exec.envp[index_pwd] = *path;
	}
}

int	is_root(t_minishell *minishell, char **path)
{
	char	*path_home;
	int		result;

	path_home = NULL;
	result = cpy_strvindex(&path_home, minishell->exec.envp, "HOME=");
	if (result == 1)
		return (1);
	else if (result == -1)
		return (0);
	*path = ft_strdup(path_home);
	if (!*path)
	{
		free(path_home);
		return (1);
	}
	free(path_home);
	return (0);
}

void	replace_oldpwd(t_minishell *minishell)
{
	char	*path_pwd;
	int		result;

	path_pwd = NULL;
	result = cpy_strvindex(&path_pwd, minishell->exec.envp, "PWD=");
	if (result == 1)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	else if (result == -1)
	{
		path_pwd = ft_strdup(minishell->builtin.pwd.result);
		if (!path_pwd)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
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

void	root_with_folder(t_minishell *minishell, char **path)
{
	char	*home;
	char	*folder;
	int		result;

	result = cpy_strvindex(&home, minishell->exec.envp, "HOME=");
	if (result == 1)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	else if (result == -1)
		return ;
	folder = ft_substr(*path, 1, ft_strlen(*path) - 1);
	if (!folder)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	free(*path);
	*path = ft_strjoin(home, folder);
	free(folder);
	free(home);
}

void	is_pwd_in_env(t_minishell *minishell)
{
	int		is_pwd;

	is_pwd = strv_searchindex(minishell->exec.envp, "PWD=");
	if (is_pwd == -1)
	{
		
	}
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
	char	*path;
	int		error;

	path = NULL; 
	if (!pipe->cmd->cmd_args || !pipe->cmd->cmd_args[0])
	{
		if (is_root(minishell, &path) == 1)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	else if (pipe->cmd->cmd_args[1])
	{
		error_cmd_args("cd", NULL, "too many arguments");
		minishell->exec.error = 1;
		return (0);
	}
	else if (pipe->cmd->cmd_args[0])
	{
		path = ft_strdup(pipe->cmd->cmd_args[0]);
		if (!path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		if (path[0] == '~')
			root_with_folder(minishell, &path);
		ft_printf_fd(2, "strdup path = %s\n", path);
		if (ft_strcmp(path, "./") == 0)
			return (0);
		ft_printf_fd(2, " c est bien restitue = %s\n", path);
	}
	error = chdir(path);
	if (error != 0)
	{
		ft_printf_fd(2, "minishell: cd: ");
		perror(path);
		minishell->exec.error = 1;
		free(path);
	}
	else
	{
		path_kind(minishell, &path);
		ft_printf_fd(2, "error = %d\n", error);
		ft_printf_fd(2, "path = %s\n", path);
		is_pwd_in_env(minishell);
		replace_oldpwd(minishell);
		if (minishell->builtin.cd.path != STAY)
			replace_pwd(minishell, &path);
		else
			free(path);
	}
	return (0);
}

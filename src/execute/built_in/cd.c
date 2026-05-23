/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:58:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/23 16:56:53 by stkloutz         ###   ########.fr       */
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

	result = cpy_strvindex(&minishell->builtin.cd.result,
			minishell->exec.envp, "HOME=");
	if (result == 1)
		return (1);
	else if (result == -1)
	{
		len = root_from_pwd(minishell->builtin.pwd.result);
		minishell->builtin.cd.result = ft_substr(minishell->builtin.pwd.result,
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
	if (pipe->cmd->cmd_args && pipe->cmd->cmd_args[0]
		&& !ft_strcmp(pipe->cmd->cmd_args[0], "."))
		init_pwd(minishell);
	if (minishell->builtin.pwd.result)
	{
		path_pwd = ft_substr(minishell->builtin.pwd.result, 4,
				ft_strlen(minishell->builtin.pwd.result) - 4);
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
	free(path_pwd);
	if (!minishell->exec.envp[result])
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
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
	folder = ft_substr(minishell->builtin.cd.result, 1,
			ft_strlen(minishell->builtin.cd.result) - 1);
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

int	cd_get_args(t_minishell *minishell, t_pipe *pipe)
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
			pwd_update(minishell);
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
	int	i;

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

void	error_getcwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*pwd;
	int		len;

	minishell->builtin.cd.error = 1;
	len = ft_strlen(minishell->builtin.pwd.result);
	if (minishell->builtin.pwd.result[len - 1] != '/')
		pwd = ft_strjoin(minishell->builtin.pwd.result, "/");
	else
		pwd = ft_strdup(minishell->builtin.pwd.result);
	if (!pwd)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	minishell->builtin.pwd.result = ft_strjoin(pwd, pipe->cmd->cmd_args[0]);
	if (!minishell->builtin.pwd.result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_printf_fd(2, "minishell: cd: error retrieving current directory: ");
	ft_printf_fd(2, "getcwd: cannot access parent directories: ");
	ft_printf_fd(2, "No such file or directory\n");
	free(pwd);
}

int	is_pwd_invalid(void)
{
	char	*is_pwd;

	is_pwd = getcwd(NULL, 0);
	if (!is_pwd)
	{
		free(is_pwd);
		return (1);
	}
	free(is_pwd);
	return (0);
}

static int	remove_lastfolder(char **old_pwd, int last_slash)
{
	char	*tmp;

	tmp = ft_strdup(*old_pwd);
	if (!tmp)
		return (1);
	free(*old_pwd);
	*old_pwd = ft_substr(tmp, 0, last_slash);
	if (!*old_pwd)
		return (1);
	free(tmp);
	return (0);
}

static int	path_replacefolder(char **oldpwd, t_builtin_content *cd)
{
	int		last_slash;

	last_slash = index_lastchar(*oldpwd, '/');
	if (last_slash > 0)
	{
		if (has_alpha(cd->result))
		{
			if (remove_lastfolder(oldpwd, last_slash)
				|| join_oldnew(oldpwd, &cd->result))
				return (1);
		}
		else
		{
			if (remove_lastfolder(oldpwd, last_slash)
				|| str_copy_and_free(oldpwd, &cd->result))
				return (1);
		}
	}
	return (0);
}

static void	remove_dir(t_minishell *minishell, t_builtin_content *cd)
{
	char	*old_pwd;

	if (!ft_strnstr(cd->result, "../", ft_strlen(cd->result)))
		return ;
	old_pwd = ft_substr(minishell->builtin.pwd.result, 4,
			ft_strlen(minishell->builtin.pwd.result));
	if (!old_pwd || path_replacefolder(&old_pwd, cd))
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	free(old_pwd);
}

void	free_and_set_null(t_minishell *minishell)
{
	free(minishell->builtin.cd.result);
	minishell->builtin.cd.result = NULL;
}

void	print_error_cd(t_minishell *minishell)
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

	if (minishell->builtin.cd.result)
		free_and_set_null(minishell);
	if (cd_get_args(minishell, pipe))
		return (0);
	remove_dir(minishell, &minishell->builtin.cd);
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

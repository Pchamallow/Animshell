/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_root.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:47:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/06 16:57:47 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** 1. check parent folder permission
** 2. is path searched inclued parent folder ?
*/
int	is_perm_parentfolder(t_builtin_content *cd, char *oldpwd, char *original)
{
	char	*parent;

	parent = ft_strdup(oldpwd);
	if (!parent)
		return (-1);
	remove_lastfolder(&parent, index_lastchar(parent, '/'));
	if (access(parent, X_OK) == -1)
	{
		if (!ft_strnstr(cd->result, oldpwd, ft_strlen(cd->result)))
		{
			free(cd->result);
			cd->result = ft_strdup(original);
			free(parent);
			free(oldpwd);
			if (!cd->result)
				return (-1);
			return (0);
		}
	}
	free(parent);
	free(oldpwd);
	return (1);
}

/*
** if parent folder = no permission
**			-> cant ../
**			-> if original include ../ :
**				- keep original ../ in order to chdir print error
**			-> else :
**				- we move in sub folder
**				- keep absolute path
** if actual folder have permission access
**			-> keep original path, eg: "../"
**			-> else : keep absolute path, eg: "/home/documents"
*/
int	dir_permission(t_minishell *minishell, t_builtin_content *cd,
		char *original)
{
	char	*oldpwd;
	int		perm_parentfolder;

	oldpwd = init_oldpwd(minishell);
	if (!oldpwd)
		return (1);
	perm_parentfolder = is_perm_parentfolder(cd, oldpwd, original);
	if (perm_parentfolder == -1)
		return (1);
	else if (!perm_parentfolder)
		return (0);
	if (access(cd->result, X_OK))
	{
		free(cd->result);
		cd->result = ft_strdup(original);
		if (!cd->result)
			return (1);
	}
	return (0);
}

int	root(t_minishell *minishell, char **home)
{
	int	result;

	result = cpy_strvindex(home, minishell->exec.envp, "HOME=");
	if (result == 1)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	else if (result == -1)
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		minishell->exec.error = 1;
		return (0);
	}
	return (1);
}

int	root_with_folder(t_minishell *minishell)
{
	char	*home;
	char	*folder;

	if (!root(minishell, &home))
		return (0);
	folder = ft_substr(minishell->builtin.cd.result, 1,
			ft_strlen(minishell->builtin.cd.result) - 1);
	if (!folder)
	{
		free(home);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(minishell->builtin.cd.result);
	minishell->builtin.cd.result = ft_strjoin(home, folder);
	free(folder);
	free(home);
	return (1);
}

int	check_args(t_minishell *minishell, t_pipe *pipe)
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
	{
		if (!root_with_folder(minishell))
			return (1);
	}
	if (!ft_strcmp(minishell->builtin.cd.result, "./"))
		return (0);
	return (0);
}

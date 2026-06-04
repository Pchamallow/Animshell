/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_root.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:47:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/04 13:25:54 by pswirgie         ###   ########.fr       */
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

static int	end_root(char *path)
{
	int	folder;
	int	slash;
	int	i;

	i = 0;
	folder = 0;
	slash = 0;
	while (path[i] && slash < 2)
	{
		if (path[i] && path[i] == '/')
		{
			slash++;
			while (path[i] && path[i] == '/')
				i++;
		}
		if (path[i] && path[i] != '/')
		{
			folder++;
			while (path[i] && path[i] != '/')
				i++;
		}
	}
	if (slash >= 2 && folder > 0)
		return (i);
	return (-1);
}

char	*root(void)
{
	char	*root;
	char	*path;
	int		i;

	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	i = end_root(path);
	if (i != -1)
		root = ft_substr(path, 0, i);
	else
		root = ft_strdup(path);
	if (!root)
	{
		free(path);
		return (NULL);
	}
	free(path);
	return (root);
}

void	root_with_folder(t_minishell *minishell)
{
	char	*home;
	char	*folder;

	home = root();
	if (!home)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
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
}

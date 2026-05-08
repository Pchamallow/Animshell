/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path_clean.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:24:30 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 11:36:39 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* remove "./" */
static int	remove_relativepart(char **path)
{
	char	*tmp;

	tmp = ft_substr(*path, 2, ft_strlen(*path) + 1);
	if (!tmp)
		return (1);
	free(*path);
	*path = ft_strdup(tmp);
	free(tmp);
	return (0);
}

static int	path_replacefolder(char **old_pwd, char **path)
{
	int		last_slash;

	last_slash = index_lastchar(*old_pwd, '/');
	// printf("old pwd ici == %c\n", (*old_pwd)[last_slash]);// test
	// printf("old pwd index fin == %d\n", last_slash);// test
	if (last_slash > 0)
	{
		if (remove_relativepart(path))
			return (1);
		if (has_alpha(*path))
		{
			if (remove_lastfolder(old_pwd, last_slash)
				|| join_oldnew(old_pwd, path))
				return (1);
		}
		else
		{
			if (remove_lastfolder(old_pwd, last_slash)
				|| str_copy_and_free(old_pwd, path))
				return (1);
		}
	}
	return (0);
}

// char	*path_replacefolder(char *old_pwd, char **path)
// {
// 	char	*new_pwd;
// 	char	*tmp;
// 	int		max_old;

// 	max_old = ft_strlen(old_pwd) - 1; // verifier qu il va jusqu bout
// 	// if (!find_ascii(old_pwd))
// 		//suppr le dernier folder sans le remplacer
// 		// return
// 	new_pwd = NULL;
// 	// le / de fin n est pas cens'e exister, il ne doit pas il y avoir
// 	while (old_pwd[max_old]
// 		&& old_pwd[max_old] != '/')
// 		max_old--;
// 	if (max_old > 0)
// 	{
// 		new_pwd = ft_substr(old_pwd, 0, max_old);
// 		// new_path = ft_substr
// 		tmp = ft_strdup(new_pwd);
// 		free(new_pwd);
// 		new_pwd = ft_strjoin(tmp, *path);
// 	}
// 	return (new_pwd);
// }

/*
** PATH_CLEAN *********************************************
** 1. if needed, remove several / 
** 		- if we have a alpha remove the / or // at end of path,
		- begin .// -> /
** 2. clean the path by kend
**		- absolute (path = /home/documents)
**			new_pwd = "PWD=" + path
**		- relative single (path = ./documents)
**			new_pwd = old_pwd + path (without ./)
**		- relative double (path = ../documents)
**			new_pwd = old_pwd (without last folder)
**						+ path (without ./)
** 
*/
int	path_clean(t_minishell *minishell, char **path, int index_pwd)
{
	t_path_kind kind;
	char		*new_path;
	char		*old_pwd;

	new_path = NULL;
	kind = minishell->builtin.cd.path;
	if (index_pwd == -1)
		old_pwd = ft_strdup(minishell->builtin.pwd.result);
	else 
		old_pwd = ft_strdup(minishell->exec.envp[index_pwd]);
	if (!old_pwd)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	if (has_alpha(*path))
	{
		if (remove_begin(path, '/') || remove_end(path, '/'))
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	// mettre cette a part la recherche de ./ dans remove begin pour conserve la fonction modulable dans tous le projet
	}
	// printf("kind == %d\n", kind);
	if (kind == ABSOLUTE)
	{
		// printf("path original == %s\n", *path);
		new_path = ft_strjoin("PWD=", *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(*path);
		*path = new_path;
		// printf("path absolute == %s\n", *path);
	}
	else if (kind == RELATIVE_SINGLE)
	{
		new_path = ft_strjoin(old_pwd, *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(*path);
		*path = new_path;
	}
	else if (kind == RELATIVE_DOUBLE)
	{
		if (path_replacefolder(&old_pwd, path))
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	if (kind != RELATIVE_DOUBLE)
		free(old_pwd);
	return (0);
}

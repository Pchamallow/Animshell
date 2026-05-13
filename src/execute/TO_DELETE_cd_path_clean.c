/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TO_DELETE_cd_path_clean.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:24:30 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/13 12:25:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	path_kind(t_minishell *minishell, char **path)
// {
// 	char	*new_path;

// 	if ((*path)[0] == '.' && (*path)[1] == '.' && (*path)[2] == '/')
// 		minishell->builtin.cd.path = RELATIVE_DOUBLE;
// 	else if ((*path)[0] == '.' && (*path)[1] == '/')
// 		minishell->builtin.cd.path = RELATIVE_SINGLE;
// 	else if ((*path)[0] == '/')
// 		minishell->builtin.cd.path = ABSOLUTE;
// 	else if ((*path)[0] == '.' && !(*path)[1])
// 		minishell->builtin.cd.path = STAY;
// 	else if (ft_isalpha((*path)[0]))
// 	{
// 		new_path = ft_strjoin("./", *path);
// 		if (!new_path)
// 			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 		free(*path);
// 		*path = new_path;
// 		minishell->builtin.cd.path = RELATIVE_SINGLE;
// 	}
// }

// static int	remove_lastfolder(char **old_pwd, int last_slash)
// {
// 	char	*tmp;

// 	tmp = ft_strdup(*old_pwd);
// 	if (!tmp)
// 		return (1);
// 	free(*old_pwd);
// 	*old_pwd = ft_substr(tmp, 0, last_slash);
// 	if (!*old_pwd)
// 		return (1);
// 	free(tmp);
// 	return (0);
// }

// /* remove "./" */
// static int	remove_relativepart(char **path)
// {
// 	char	*tmp;

// 	tmp = ft_substr(*path, 2, ft_strlen(*path) + 1);
// 	if (!tmp)
// 		return (1);
// 	free(*path);
// 	*path = ft_strdup(tmp);
// 	free(tmp);
// 	return (0);
// }

// static int	path_replacefolder(char **old_pwd, char **path)
// {
// 	int		last_slash;

// 	last_slash = index_lastchar(*old_pwd, '/');
// 	if (last_slash > 0)
// 	{
// 		if (remove_relativepart(path))
// 			return (1);
// 		if (has_alpha(*path))
// 		{
// 			if (remove_lastfolder(old_pwd, last_slash)
// 				|| join_oldnew(old_pwd, path))
// 				return (1);
// 		}
// 		else
// 		{
// 			if (remove_lastfolder(old_pwd, last_slash)
// 				|| str_copy_and_free(old_pwd, path))
// 				return (1);
// 		}
// 	}
// 	return (0);
// }

// void	cd_remove_duplicate(t_minishell *minishell, char **path)
// {
// 	if (has_alpha(*path))
// 	{
// 		if ((*path)[0] == '.' && (*path)[1] == '/')
// 		{
// 			if (remove_begin(path, '/', 2, 2) || remove_end(path, '/'))
// 				print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 		}
// 		else
// 		{
// 			if (remove_begin(path, '/', 0, 0) || remove_end(path, '/'))
// 				print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 		}
// 	}
// }

// // void	cd_modify_path

// /*
// ** PATH_CLEAN *********************************************
// ** 1. cd_remove_duplicate =
// **	 if needed, remove several / 
// ** 		- if we have a alpha remove the / or // at end of path,
// **		- begin .// -> /
// ** 2. clean the path
// **		- absolute (path = /home/documents)
// **			new_pwd = "PWD=" + path
// **		- relative single (path = ./documents)
// **			new_pwd = old_pwd + path (without ./)
// **		- relative double (path = ../documents)
// **			new_pwd = old_pwd (without last folder)
// **						+ path (without ./)
// */
// int	path_clean(t_minishell *minishell, char **path, int index_pwd)
// {
// 	t_path_kind kind;
// 	char		*new_path;
// 	char		*old_pwd;

// 	(void)index_pwd;
// 	new_path = NULL;
// 	kind = minishell->builtin.cd.path;
// 	if (minishell->builtin.pwd.result)
// 	{
// 		old_pwd = ft_strdup(minishell->builtin.pwd.result);
// 		if (!old_pwd)
// 			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 	}
// 	else
// 		return (1);
// 	cd_remove_duplicate(minishell, path);
// 	// cd_modify_path(minishell, path);
// 	if (kind == ABSOLUTE)
// 	{
// 		new_path = ft_strjoin("PWD=", *path);
// 		if (!new_path)
// 			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 		free(*path);
// 		*path = new_path;
// 	}
// 	else if (kind == RELATIVE_SINGLE)
// 	{
// 		new_path = ft_strjoin(old_pwd, *path);
// 		if (!new_path)
// 			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 		free(*path);
// 		*path = new_path;
// 	}
// 	else if (kind == RELATIVE_DOUBLE)
// 	{
// 		if (path_replacefolder(&old_pwd, path))
// 			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 	}
// 	if (kind != RELATIVE_DOUBLE)
// 		free(old_pwd);
// 	return (0);
// }


// static void	replace_pwd(t_minishell *minishell, char **path)
// {
// 	int		index_pwd;

// 	return ;
// 	index_pwd = strv_searchindex(minishell->exec.envp, "PWD=");
// 	path_clean(minishell, path, index_pwd);
// 	if (minishell->builtin.pwd.result)
// 		free(minishell->builtin.pwd.result);
// 	minishell->builtin.pwd.result = ft_strdup(*path);
// 	if (!minishell->builtin.pwd.result)
// 		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 	if (index_pwd != -1)
// 	{
// 		free(minishell->exec.envp[index_pwd]);
// 		minishell->exec.envp[index_pwd] = ft_strdup(*path);
// 	}
// 	if (*path)
// 		free(*path);
// }
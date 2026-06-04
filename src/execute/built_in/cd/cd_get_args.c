/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_get_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:50:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/04 13:41:04 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_lastfolder(char **new_path, int last_slash)
{
	char	*result;

	result = ft_substr(*new_path, 0, last_slash);
	free(*new_path);
	if (!result)
		return (1);
	*new_path = result;
	return (0);
}

static int	add_folder(char **new_path, char *path_to_find, int *len)
{
	char	*folder;
	char	*res;

	res = ft_strdup(*new_path);
	if (!res)
		return (1);
	free(*new_path);
	*len = strfind(path_to_find, "/", 1);
	if (*len == -1)
		*len = ft_strlen(path_to_find);
	folder = ft_substr(path_to_find, 0, *len);
	if (!folder)
		return (1);
	folder = join_oldnew("/", folder, true);
	if (!folder)
		return (1);
	res = join_oldnew(folder, res, false);
	free(folder);
	if (!res)
		return (1);
	*new_path = res;
	return (0);
}

static int	convert_to_absolute(char **new_path, char *path_to_find, int i)
{
	int	error;
	int	len;

	len = 0;
	error = 0;
	if (i >= (int)ft_strlen(path_to_find))
		return (0);
	else if (strfind(&path_to_find[i], "../", 0) == 0)
	{
		if (remove_lastfolder(new_path, index_lastchar(*new_path, '/')))
			return (1);
		error = convert_to_absolute(new_path, path_to_find, i + 3);
	}
	else if (strfind(&path_to_find[i], "./", 0) == 0)
		error = convert_to_absolute(new_path, path_to_find, i + 2);
	else if (path_to_find[i] != '/')
	{
		if (add_folder(new_path, &path_to_find[i], &len))
			return (1);
		error = convert_to_absolute(new_path, path_to_find, i + len);
	}
	else if (path_to_find[i] == '/')
		error = convert_to_absolute(new_path, path_to_find, i + 1);
	return (error);
}

static int	path_replacefolder(char *oldpwd, t_builtin_content *cd)
{
	char	*path_to_find;
	char	*new_path;

	path_to_find = ft_strdup(cd->result);
	if (!path_to_find)
		return (1);
	new_path = ft_strdup(oldpwd);
	if (!new_path)
		return (1);
	free(cd->result);
	cd->result = NULL;
	if (convert_to_absolute(&new_path, path_to_find, 0))
	{
		free(new_path);
		free(path_to_find);
		return (1);
	}
	cd->result = ft_strdup(new_path);
	free(new_path);
	free(path_to_find);
	return (0);
}

/*
** if args begin by "../" -> check permissions folder
** no permissions = convert pwd in absolute path 
** eg : arg = ../folder ->
**					pwd	= /home/documents/tests
**					cd	= /home/documents/folder
*/
void	is_perm_folder(t_minishell *minishell, t_builtin_content *cd)
{
	char	*old_pwd;
	char	*original;

	if (!ft_strnstr(cd->result, "../", ft_strlen(cd->result)))
		return ;
	original = ft_strdup(cd->result);
	if (!original)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	old_pwd = init_oldpwd(minishell);
	if (!old_pwd || path_replacefolder(old_pwd, cd))
	{
		free(old_pwd);
		free(original);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(old_pwd);
	if (dir_permission(minishell, cd, original))
	{
		free(original);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(original);
}

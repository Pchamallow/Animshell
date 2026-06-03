/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_get_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:50:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/03 11:00:48 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_lastfolder(char **new_path, int last_slash)
{
	char	*result;

	result = ft_substr(*new_path, 0, last_slash);
	free(*new_path);
	if (!result)
		return (1);
	*new_path = result;
	return (0);
}

/*
** if folder have permission access -> keep original path, eg: "../"
** else keep full path				-> eg: "/home/documents"
*/
static int	dir_permission(t_builtin_content *cd, char *original)
{
	if (access(cd->result, X_OK))
	{
		free(cd->result);
		cd->result = ft_strdup(original);
		if (!cd->result)
			return (1);
	}
	return (0);
}

int	end_root(char *path)
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
			while(path[i] && path[i] != '/')
				i++;
		}
	}
	if (slash >= 2 && folder > 0)
		return (i);
	return (-1);
}

char	*root()
// penser a ajouter ca dans root_with_folder
//gere si root est null
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
		return (NULL);
	return (root);
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

static void	handle_root(char **new_path)
{
	if (strfind_occurences(*new_path, "/", 0) < 2 && !has_alpha(*new_path))
	{
		free(*new_path);
		*new_path = root();
	}
}

static int	convert_to_absolute(char **new_path, char *path_to_find, int i)
{
	ft_printf_fd(2, "path_to_find = %s\n", &path_to_find[i]);// a suppr
	ft_printf_fd(2, "new_path = %s\n", *new_path);// a suppr
	ft_printf_fd(2, "i = %d\n", i);// a suppr
	int	error;
	int	len;

	if (i >= (int)ft_strlen(path_to_find))
	{
		handle_root(new_path);
		return (0);
	}
	else if (strfind(&path_to_find[i], "../", 0) == 0)
	{
		if (remove_lastfolder(new_path, index_lastchar(*new_path, '/')))
			return (1);
		error = convert_to_absolute(new_path, path_to_find, i + 3);
	}
	else if (strfind(&path_to_find[i], "./", 0) == 0)
		error = convert_to_absolute(new_path, path_to_find, i + 2);
	else if (ft_isalpha(path_to_find[i]))
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
	char	*path_to_find = ft_strdup(cd->result);
	if (!path_to_find)
		return (1);
	char	*new_path = ft_strdup(oldpwd);
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
	// si on commence a partir de l index 0 par telle str
	// chercher str, retourner l index de debut du find
}

/*
** if args begin by "../" -> check permissions folder
** no permissions = convert pwd in absolute path 
** eg : arg = ../folder ->
**					pwd	= /home/documents/tests
**					cd	= /home/documents/folder
*/
void	remove_dir(t_minishell *minishell, t_builtin_content *cd)
{
	char	*old_pwd;
	char	*original;

	if (!ft_strnstr(cd->result, "../", ft_strlen(cd->result)))
		return ;
	original = ft_strdup(cd->result);
	if (!original)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	old_pwd = ft_substr(minishell->builtin.pwd.result, 4,
			ft_strlen(minishell->builtin.pwd.result));
	if (!old_pwd || path_replacefolder(old_pwd, cd))
	{
		free(old_pwd);
		free(original);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(old_pwd);
	if (dir_permission(cd, original))
	{
		free(original);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(original);
}

/*
static int	path_replacefolder(char **oldpwd, t_builtin_content *cd)
{
	int	i;
	int	last_slash;
	int	back;
	int	len;

	i = 0;
	last_slash = 0;
	back = 0;
	char *original = ft_strdup(cd->result);
	len = ft_strlen(original);
	while (original[i])
	{
		if (ft_strcmp(&original[i], "../"))
		{
			i += 3;
			back++;
			while (i < len && ft_strcmp(&original[i], "../"))
			{
				i += 3;
				back++;
			}
			while (back > 0)
			{
				last_slash = index_lastchar(*oldpwd, '/');
				if (remove_lastfolder(oldpwd, last_slash))
					return (1);
				back--;
			}
		}
		else if (original[i] == '/')
		{
			while (original[i] && original[i] == '/')
				i++;
		}
		else
		{
			if (join_oldnew(*oldpwd, &original[i]))
				return (1);
		}
		i++;
	}
	free(cd->result);
	cd->result = original;
	return (0);
}
*/
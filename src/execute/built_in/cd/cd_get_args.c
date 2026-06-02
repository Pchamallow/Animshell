/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_get_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:50:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 18:36:25 by pswirgie         ###   ########.fr       */
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
	if (!old_pwd || path_replacefolder(&old_pwd, cd))
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
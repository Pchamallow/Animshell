/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_get_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:50:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 15:08:25 by pswirgie         ###   ########.fr       */
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

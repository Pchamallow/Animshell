/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:10:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/06 12:21:13 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
copy a new
free new
malloc new with a new len
*/
// CONVERTIR LE NEW EN ** POUR NE PAS EN FAIRE UNE COPIE SEULEEMNT
// char	*str_cpyinit(char *original, char *new, int new_len)
// {
// 	if (new)
// 	{
// 		original = ft_strdup(new);
// 		if (!original)
// 			return (NULL);
// 		free(new);
// 		new = ft_calloc(new_len, sizeof(char));
// 		if (!new)
// 		{
// 			free(original);
// 			return (NULL);
// 		}
// 		ft_printf_fd(2, " c est bien copi'e = %s\n", original);
// 		return (new);
// 	}
// 	return (NULL);
// }

static void	path_kind(t_minishell *minishell, char **path)
{
	char	*new_path;

	if ((*path)[0] == '.' && (*path)[1] == '.' && (*path)[2] == '/')
		minishell->builtin.cd.path = RELATIVE_DOUBLE;
	else if ((*path)[0] == '.' && (*path)[1] == '/')
		minishell->builtin.cd.path = RELATIVE_SINGLE;
	else if ((*path)[0] == '/')
		minishell->builtin.cd.path = ABSOLUTE;
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

static char	**init_new_envp(char **envp)
{
	int		len;
	char	**new_envp;

	len = 0;
	while (envp[len])
		len++;
	new_envp = ft_calloc(len + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	new_envp[len] = NULL;
	return (new_envp);
}

static char	*path_clean(t_minishell *minishell, char *path, int index_pwd)
{
	t_path_kind kind;
	char		*new_path;
	char		*old_pwd;

	new_path = NULL;
	kind = minishell->builtin.cd.path;
	if (kind == ABSOLUTE)
	{
		new_path = safe_join("PWD=", path);
		return (new_path);
	}
	else if (kind == RELATIVE_SINGLE)
	{
		old_pwd = ft_strdup(&minishell->exec.envp[index_pwd][4]);
		if (!old_pwd)
			return (NULL);
		new_path = safe_join(old_pwd, &path[2]);
		return (new_path);
	}
	return (new_path);
}

static void	replace_pwd(t_minishell *minishell, char *path)
{
	char	**new_envp;
	char	*new_path;
	int		index_pwd;
	int		i;

	i = 0;
	index_pwd = find_pwd(minishell);
	new_path = path_clean(minishell, path, index_pwd);
	if (!new_path)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	if (index_pwd != -1)
	{
		new_envp = init_new_envp(minishell->exec.envp);
		while (i < index_pwd)
		{
			new_envp[i] = minishell->exec.envp[i];
			i++;
		}
		free(minishell->exec.envp[i]);
		new_envp[i] = new_path;
		i++;
		while (minishell->exec.envp[i])
		{
			new_envp[i] = minishell->exec.envp[i];
			i++;
		}
		free(minishell->exec.envp);
		minishell->exec.envp = new_envp;
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
		error_cmd_args("cd", NULL, "require one argument");
		minishell->exec.error = 1;
	}
	else if (pipe->cmd->cmd_args[1])
	{
		error_cmd_args("cd", NULL, "too many arguments");
		minishell->exec.error = 1;
	}
	else if (pipe->cmd->cmd_args[0])
	{
		path = ft_strdup(pipe->cmd->cmd_args[0]);
		if (!path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		// if (ft_strcmpp(path, "//") == 0)
		path_kind(minishell, &path);
		ft_printf_fd(2, " c est bien restitue = %s\n", path);
		error = chdir(path);
		if (error != 0)
		{
			ft_printf_fd(2, "minishell: cd: ");
			perror(path);
			minishell->exec.error = 1;
		}
		else
			replace_pwd(minishell, path);
		ft_printf_fd(2, "error = %d\n", error);
		ft_printf_fd(2, "path = %s\n", path);
		free(path);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:10:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 19:24:11 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	replace_pwd(t_minishell *minishell, char *path)
{
	char	**new_envp;
	char	*new_path;
	char	*base;
	int		pwd;
	int		i;

	i = 0;
	pwd = find_pwd(minishell);
	base = ft_strdup("PWD=");
	new_path = ft_strjoin(base, path);
	free(base);
	if (pwd != -1)
	{
		new_envp = init_new_envp(minishell->exec.envp);
		while (i < pwd)
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
** CD ************************************
** Move the current position in a
** directory specified.
** - only a relative or absolute path,
** like required by the subject
** - no argument or > 1= error message
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
		error = chdir(path);
		if (chdir(path) != 0)
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

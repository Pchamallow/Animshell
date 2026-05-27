/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_update_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:45:50 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 16:23:21 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pwd_invalid(void)
{
	char	*is_pwd;

	is_pwd = getcwd(NULL, 0);
	if (!is_pwd)
	{
		free(is_pwd);
		return (1);
	}
	free(is_pwd);
	return (0);
}

void	error_getcwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*pwd;
	int		len;

	minishell->builtin.cd.error = 1;
	len = ft_strlen(minishell->builtin.pwd.result);
	if (minishell->builtin.pwd.result[len - 1] != '/')
		pwd = ft_strjoin(minishell->builtin.pwd.result, "/");
	else
		pwd = ft_strdup(minishell->builtin.pwd.result);
	if (!pwd)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	minishell->builtin.pwd.result = ft_strjoin(pwd, pipe->cmd->cmd_args[0]);
	if (!minishell->builtin.pwd.result)
	{
		free(pwd);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	ft_printf_fd(2, "minishell: cd: error retrieving current directory: ");
	ft_printf_fd(2, "getcwd: cannot access parent directories: ");
	ft_printf_fd(2, "No such file or directory\n");
	free(pwd);
}

static char	*find_oldpwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*result;

	result = NULL;
	if (pipe->cmd->cmd_args && pipe->cmd->cmd_args[0]
		&& !ft_strcmp(pipe->cmd->cmd_args[0], "."))
		pwd_init(minishell);
	if (minishell->builtin.pwd.result)
	{
		result = ft_substr(minishell->builtin.pwd.result, 4,
				ft_strlen(minishell->builtin.pwd.result) - 4);
		if (!result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	return (result);
}

void	replace_oldpwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*path_pwd;
	int		result;
	int		len_envp;

	path_pwd = find_oldpwd(minishell, pipe);
	result = strv_searchindex(minishell->exec.envp, "OLDPWD=");
	if (result == -1)
	{
		if (path_pwd)
			free(path_pwd);
		return ;
	}
	len_envp = strvlen(minishell->exec.envp);
	free(minishell->exec.envp[result]);
	minishell->exec.envp[result] = ft_strjoin("OLDPWD=", path_pwd);
	free(path_pwd);
	if (!minishell->exec.envp[result])
	{
		free_strv_len(minishell->exec.envp, len_envp);
		minishell->exec.envp = NULL;
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
}

/*
** if PWD unset, return -> not create variable PWD
** else search index PWD in envp, free and cpy pwd
*/
void	modify_pwd_in_envp(t_minishell *minishell)
{
	int	i;

	if (strv_searchindex(minishell->exec.envp, "PWD=") == -1)
		return ;
	pwd_init(minishell);
	i = strv_searchindex(minishell->exec.envp, "PWD=");
	if (i != -1)
	{
		free(minishell->exec.envp[i]);
		minishell->exec.envp[i] = ft_strdup(minishell->builtin.pwd.result);
		if (!minishell->exec.envp[i])
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
}

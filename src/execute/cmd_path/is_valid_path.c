/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/01 15:44:18 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_path(t_minishell *minishell, t_token *token)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	if (!minishell->exec.paths_for_search_cmd || !has_alpha(token->value))
		return (-1);
	len = strvlen(minishell->exec.paths_for_search_cmd);
	while (minishell->exec.paths_for_search_cmd[i] && i <= len)
	{
		tmp = ft_strjoin(minishell->exec.paths_for_search_cmd[i], "/");
		token->cmd_path = ft_strjoin(tmp, token->value);
		free(tmp);
		if (access(token->cmd_path, X_OK) == 0)
			return (0);
		free(token->cmd_path);
		token->cmd_path = NULL;
		i++;
	}
	return (-1);
}

int	is_directory(t_minishell *minishell, t_pipe *pipe, char *str)
{
	char	*dir;
	DIR		*is_dir;

	dir = str;
	is_dir = opendir(dir);
	if (is_dir)
	{
		closedir(is_dir);
		error_cmd_args(str, NULL, "Is a directory");
		if (pipe->is_cmd)
			minishell->exec.error = 1;
		else
			minishell->exec.error = 126;
		return (1);
	}
	return (0);
}

/*
Command not found = -1;
To search path = 0;
Absolute path = 1;
Explicit path = 2;
*/
static int	path_type(t_minishell *minishell, t_pipe *pipe, char *token)
{
	if (token[0] == '/')
	{
		if (is_directory(minishell, pipe, token))
			return (-1);
		if (access(token, X_OK) == 0)
			return (1);
		else
		{
			error_cmd_args(token, NULL, "No such file or directory");
			if (pipe->is_cmd)
				minishell->exec.error = 1;
			else
				minishell->exec.error = 127;
			return (-1);
		}
	}
	if (token[0] == '.' && token[1] == '/')
		return (2);
	return (0);
}

static int	error_path(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	if (is_directory(minishell, pipe, token->value))
		return (1);
	if (access(token->value, X_OK) == 0)
	{
		path_explicit(minishell, token);
		return (0);
	}
	else
	{
		if (access(token->value, F_OK) != 0)
		{
			error_cmd_args(token->value, NULL, "No such file or directory");
			minishell->exec.error = 127;
		}
		if (access(token->value, R_OK) != 0)
		{
			error_cmd_args(token->value, NULL, "Permission denied");
			minishell->exec.error = 126;
		}
		return (1);
	}
}

/*
-1 = / path + no such file or directory
0 = cmd, path to find
1 = / path + cmd valid
2 = ./ path
*/
int	path_cmd(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	int		i;
	int		len;

	i = path_type(minishell, pipe, token->value);
	if (i == 1)
	{
		len = len_cmd_no_endspace(token->value) + 1;
		token->cmd_path = ft_calloc(sizeof(char *), len);
		if (!token->cmd_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		ft_strlcpy(token->cmd_path, token->value, len);
	}
	else if (i == 0)
	{
		if (is_valid_path(minishell, token) == -1)
			return (-1);
	}
	else if (i == -1)
		return (1);
	else if (i == 2)
	{
		if (error_path(minishell, pipe, token))
			return (1);
	}
	return (0);
}

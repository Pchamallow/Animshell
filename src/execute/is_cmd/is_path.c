/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/10 14:16:46 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	no_cmd(t_token *token, int *error)
{
	token->type = 0;
	/*ft_printf_fd(2, "command not found: %s\n", token->value);*/
	*error = 127;
}

/*static void	path_explicit(t_minishell *minishell, t_token *token)*/
/*{*/
	/*int	len;*/

	/*len = ft_strlen(token->value);*/
	/*token->cmd_path = ft_calloc(sizeof(char *), len + 1);*/
	/*if (!token->cmd_path)*/
		/*print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);*/
	/*ft_strlcpy(token->cmd_path, token->value, len + 1);*/
	/*cmd_explicit(minishell, token);*/
/*}*/

static void	is_valid_path(t_minishell *minishell,
	t_token *token, char **all_paths, int len)
{
	char	*tmp;
	int		i;

	i = 0;
	while (all_paths[i] && i <= len)
	{
		tmp = ft_strjoin(all_paths[i], "/");
		token->cmd_path = ft_strjoin(tmp, token->value);
		free(tmp);
		if (access(token->cmd_path, X_OK) == 0)
			break ;
		free(token->cmd_path);
		i++;
	}
	if (i >= len + 1)
		no_cmd(token, &minishell->exec.error);
}

/*
Command not found = -1;
To search path = 0;
Absolute path = 1;
Explicit path = 2;
*/
static int	path_type(t_exec *exec, char *token)
{
	if (token[0] == '/')
	{
		if (access(token, X_OK) == 0)
			return (1);
		else
		{
			ft_printf_fd(2, "no such file or directory: %s\n", token);
			exec->error = 127;
			return (-1);
		}
	}
	if (token[0] == '.' && token[1] == '/')
		return (2);
	return (0);
}

int	path_cmd(t_minishell *minishell, t_token *token, char **all_paths)
{
	int		i;
	int		len;

	i = path_type(&minishell->exec, token->value);
	if (i == 1)
	{
		len = len_cmd_no_endspace(token->value) + 1;
		token->cmd_path = ft_calloc(sizeof(char *), len);
		if (!token->cmd_path)
		{
			free_double(all_paths);
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		}
		ft_strlcpy(token->cmd_path, token->value, len);
	}
	else if (i == 0)
	{
		len = len_double(all_paths);
		is_valid_path(minishell, token, all_paths, len);
	}
	else if (i == -1)
		return (1);
	else if (i == 2)
		path_explicit(minishell, token);
	return (0);
}

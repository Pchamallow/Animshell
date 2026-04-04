/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 17:51:57 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Command not found = -1;
To search path = 0;
Absolute path = 1;
Explicit path = 2;
*/



int	path_type(t_exec *exec, char *token)
{
	int	i;

	i = 0;
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

static int	path_cmd(t_exec *exec, t_token *token)
{
	char	**all_paths;
	int		i;
	int		len;

	i = path_type(exec, token->value);
	if (i == 1)
	{
		len = len_cmd(token->value) + 1;
		token->cmd_path = ft_calloc(sizeof(char *), len);
		if (!token->cmd_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		ft_strlcpy(token->cmd_path, token->value, len);
	}
	else if (i == 0)
	{
		i = is_path(parse, envp);
		all_paths = ft_split(envp[i], ':');
		len = len_double(all_paths);
		is_valid_path(parse, type, all_paths, len);
	}
	else if (i == -1)
		return (1);
	else if (i == 2)
		path_explicit(parse, type);
	return (0);
}

/*
is a command ? path is valid ?
type 0 = WORD
type 4 = IS_CMD
*/
int is_cmd(t_exec *exec, t_token *token)
{
	char	*paths;

	i = 0;
	paths = getenv("PATH");
	while(token != NULL)
	{
		if (token->type == 0 || token->type == 4)
			path_cmd(exec, token);
		token = token->next;
	}
}
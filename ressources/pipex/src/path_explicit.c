/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_explicit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:11:20 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/28 13:37:44 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	cmd_explicit(t_parse *parse, t_element *type)
{
	int		len;

	len = ft_strlen(type->cmd_path) - 2;
	free(type->cmd);
	type->cmd = ft_calloc(sizeof(char *), len + 1);
	if (!type->cmd)
		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(type->cmd, &type->cmd_path[2], len + 1);
}

void	path_explicit(t_parse *parse, t_element *type)
{
	int	len;

	len = ft_strlen(type->cmd);
	type->cmd_path = ft_calloc(sizeof(char *), len + 1);
	if (!type->cmd_path)
		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(type->cmd_path, type->cmd, len + 1);
	cmd_explicit(parse, type);
}

/*
Command not found = -1;
To search path = 0;
Absolute path = 1;
Explicit path = 2;
*/
int	path_type(t_parse *parse, t_element *type)
{
	int	i;

	i = 0;
	if (type->cmd[0] == '/')
	{
		if (access(type->cmd, X_OK) == 0)
			return (1);
		else
		{
			ft_printf_fd(2, "no such file or directory: %s\n", type->cmd);
			parse->error = 127;
			return (-1);
		}
	}
	if (type->cmd[i] == '.' && type->cmd[i + 1] == '/')
		return (2);
	return (0);
}

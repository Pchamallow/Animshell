/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:12:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/28 13:36:19 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_path(t_parse *parse, char **envp)
{
	int	i;
	int	is_path;

	i = 0;
	is_path = 0;
	while (envp[i])
	{
		if (ft_strnstr((const char *)envp[i], "PATH=", 5) != NULL)
		{
			is_path = 1;
			break ;
		}
		i++;
	}
	if (is_path == 0)
		print_error_free(parse, "PATH not found.\n", 2);
	return (i);
}

static void	no_cmd(t_parse *parse, t_element *type)
{
	type->is_cmd = 0;
	ft_printf_fd(2, "command not found: %s\n", type->cmd);
	parse->error = 127;
}

static void	is_valid_path(t_parse *parse, t_element *type,
	char **all_paths, int len)
{
	char	*tmp;
	int		i;
	int		is_find;

	i = 0;
	is_find = 0;
	while (all_paths[i] && i <= len)
	{
		tmp = ft_strjoin(all_paths[i], "/");
		type->cmd_path = ft_strjoin(tmp, type->cmd);
		free(tmp);
		if (access(type->cmd_path, X_OK) == 0)
		{
			is_find = 1;
			break ;
		}
		free(type->cmd_path);
		i++;
	}
	if (is_find == 0)
		type->cmd_path = NULL;
	free_double(all_paths, len + 1);
	if (i >= len + 1)
		no_cmd(parse, type);
}

/*
Command not found = -1;
To search path = 0;
Absolute path = 1;
Explicit path = 2;
*/
static int	path_cmd(t_parse *parse, t_element *type, char **envp)
{
	char	**all_paths;
	int		i;
	int		len;

	i = path_type(parse, type);
	if (i == 1)
	{
		len = len_cmd(type->cmd) + 1;
		type->cmd_path = ft_calloc(sizeof(char *), len);
		if (!type->cmd_path)
			print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
		ft_strlcpy(type->cmd_path, type->cmd, len);
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

void	init_args_execve(t_parse *parse, char **envp)
{
	if (parse->in.is_file == 1 && parse->in.is_cmd == 1)
	{
		if (path_cmd(parse, &parse->in, envp) == 0)
			tabs_for_execve(parse, &parse->in);
		else
			parse->in.is_cmd = 0;
	}
	if (parse->out.is_file == 1 && parse->out.is_cmd == 1)
	{
		if (path_cmd(parse, &parse->out, envp) == 0)
			tabs_for_execve(parse, &parse->out);
		else
			parse->out.is_cmd = 0;
	}
}

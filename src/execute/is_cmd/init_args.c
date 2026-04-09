/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:12:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 21:07:24 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	no_cmd(t_parse *parse, t_element *pipe)
// {
// 	pipe->is_cmd = 0;
// 	ft_printf_fd(2, "command not found: %s\n", pipe->cmd);
// 	parse->error = 127;
// }
static void	free_tab(t_minishell *minishell, char **tab)
{
	free_double(tab);
	print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
}

static void	cpy_tab_cmd(char *str, t_pipe *pipe)
{
	int	len;

	len = ft_strlen(pipe->cmd->value) + 2;
	ft_strlcpy(str, pipe->cmd->value, len);
	return ;
}

static void	cpy_tab_content(char *str, t_pipe *pipe, int index)
{
	int	len;

	if (index == 0)
	{
		cpy_tab_cmd(str, pipe);
		return ;
	}
	index -= 1;
	if (pipe->cmd->cmd_args[index])
	{
		len = ft_strlen(pipe->cmd->cmd_args[index]) + 1;
		ft_strlcpy(str, pipe->cmd->cmd_args[index], len);
	}
}

void	fill_tab(char **tab, t_pipe *pipe)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		cpy_tab_content(tab[i], pipe, i);
		i++;
	}
}

char	**init_tab(t_minishell *minishell, t_pipe *pipe)
{
	char	**tab;
	int		len;
	int		len_tab;
	int		i;
	int		nb;

	len_tab = pipe->nb_args + 2;
	i = 0;
	tab = malloc(sizeof(char *) * len_tab);
	if (!tab)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	nb = len_tab - 1;
	len = ft_strlen(pipe->cmd->value) + 1;
	while (nb)
	{
		if (i > 0)
			len = ft_strlen(pipe->cmd->cmd_args[i - 1]) + 1;
		tab[i] = ft_calloc(len, sizeof(char));
		if (!tab[i])
			free_tab(minishell, tab);
		i++;
		nb--;
	}
	tab[i] = NULL;
	return (tab);
}

void	tabs_for_execve(t_minishell *minishell, t_pipe *pipe)
{
	pipe->cmd->args_execve = init_tab(minishell, pipe);
	if (pipe->cmd->args_execve)
		fill_tab(pipe->cmd->args_execve, pipe);
}

void	init_args_execve(t_minishell *minishell, t_pipe *pipe)
{
	if (pipe->is_cmd == 1)
		tabs_for_execve(minishell, pipe);
}

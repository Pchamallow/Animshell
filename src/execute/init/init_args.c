/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:12:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 11:26:42 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	calloc_args_tab(t_minishell *minishell, t_pipe *pipe,
		char **tab, int nb)
{
	int	i;
	int	len;

	len = ft_strlen(pipe->cmd->value) + 1;
	i = 0;
	while (nb)
	{
		if (i > 0)
			len = ft_strlen(pipe->cmd->cmd_args[i - 1]) + 1;
		tab[i] = ft_calloc(len, sizeof(char));
		if (!tab[i])
		{
			free_strv(tab);
			ft_printf_fd(2, "minishell: Malloc failed !\n");
			pipe->error = 1;
			minishell->exec.error = EXIT_FAILURE;
			return (-1);
		}
		i++;
		nb--;
	}
	return (i);
}

char	**init_tab(t_minishell *minishell, t_pipe *pipe)
{
	char	**tab;
	int		len_tab;
	int		i;
	int		nb;

	len_tab = pipe->nb_args + 2;
	i = 0;
	tab = malloc(sizeof(char *) * len_tab);
	if (!tab)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	nb = len_tab - 1;
	i = calloc_args_tab(minishell, pipe, tab, nb);
	if (i < 0)
		return (NULL);
	tab[i] = NULL;
	return (tab);
}

void	init_args_execve(t_minishell *minishell, t_pipe *pipe)
{
	if (pipe->is_cmd == 1)
	{
		pipe->cmd->args_execve = init_tab(minishell, pipe);
		if (pipe->cmd->args_execve)
			fill_tab(pipe->cmd->args_execve, pipe);
	}
}

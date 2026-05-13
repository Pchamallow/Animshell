/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:11:33 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/10 12:35:30 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**envp_copy(char **envp, int len)
{
	int		i;
	char	**new_envp;

	new_envp = ft_calloc(len + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static void	sort_envp(char **sorted_envp)
{
	int		i;
	int		j;
	int		smallest;
	char	*tmp;

	i = 0;
	while (sorted_envp[i])
	{
		smallest = i;
		j = i + 1;
		while (sorted_envp[j])
		{
			if (ft_env_cmp(sorted_envp[smallest], sorted_envp[j]) > 0)
				smallest = j;
			j++;
		}
		if (smallest != i)
		{
			tmp = sorted_envp[i];
			sorted_envp[i] = sorted_envp[smallest];
			sorted_envp[smallest] = tmp;
		}
		i++;
	}
}

static void	print_envp_line(char *envp_line)
{
	int	i;

	i = 0;
	write(1, "export ", 7);
	while (envp_line[i] && envp_line[i] != '=')
	{
		write(1, &envp_line[i], 1);
		i++;
	}
	if (envp_line[i] == '=')
	{
		write(1, "=\"", 2);
		i++;
		write(1, envp_line + i, ft_strlen(envp_line + i));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

static int	print_sorted_envp(t_minishell *minishell)
{
	int		i;
	int		len;
	char	**sorted_envp;

	if (!minishell->exec.envp)
		return (0);
	len = get_envp_len(minishell->exec.envp);
	sorted_envp = envp_copy(minishell->exec.envp, len);
	if (!sorted_envp)
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	sort_envp(sorted_envp);
	i = 0;
	while (sorted_envp[i])
	{
		print_envp_line(sorted_envp[i]);
		i++;
	}
	free(sorted_envp);
	return (0);
}

/*
** EXPORT_PRINT ***************************************
** -> if export is called with no arguments,
** it prints all environment variables:
**		- in alphabetical order
**		- preceeded with "export "
**		- the variable value is enclosed with quotes
**	***************************************************
*/
int	export_print(t_minishell *minishell, t_pipe *pipe)
{
	if (pipe->nb_args == 0)
		print_sorted_envp(minishell);
	return (0);
}

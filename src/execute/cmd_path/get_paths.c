/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 14:55:34 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/09 15:32:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	get_paths_for_cmd(t_minishell *minishell)
{
	int	i;

	i = 0;
	if (minishell->exec.paths_for_search_cmd)
		free_strv(minishell->exec.paths_for_search_cmd);
	minishell->exec.paths_for_search_cmd = NULL;
	while (minishell->exec.envp[i])
	{
		if (ft_strnstr(minishell->exec.envp[i], "PATH=", 5) != NULL)
		{
			minishell->exec.paths_for_search_cmd = ft_split(minishell->exec.envp[i] + 5, ':');
			if (!minishell->exec.paths_for_search_cmd)
				print_error_free(minishell, "Error\nMalloc failed.\n", 1);;
			break;
		}
		i++;
	}
}

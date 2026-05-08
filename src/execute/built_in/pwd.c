/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 11:55:00 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pwd(t_minishell *minishell)
{
	int	i;

	i = strv_searchindex(minishell->exec.envp, "PWD=");
	if (i == -1)
	{
		minishell->builtin.pwd.result = NULL;
		return ;
	}
	minishell->builtin.pwd.result = ft_strdup(minishell->exec.envp[i]);
	if (!minishell->builtin.pwd.result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
}

int	pwd(t_minishell *minishell, t_pipe *pipe)
{
	int		i;

	i = 0;
	(void)pipe;
	if (!minishell->exec.envp || !minishell->exec.envp[0])
		return (0);
	i = strv_searchindex(minishell->exec.envp, "PWD=");
	if (i != -1)
	// if (i == -1)
		ft_printf_fd(1, "%s\n", &minishell->exec.envp[i][4]);
	else
		ft_printf_fd(1, "%s\n", &minishell->builtin.pwd.result[4]);
	return (0);
}

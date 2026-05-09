/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/09 18:08:49 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pwd(t_minishell *minishell)
{
	minishell->builtin.pwd.result = ft_strjoin("PWD=", getcwd(NULL, 0));
	if (!minishell->builtin.pwd.result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
}

int	pwd(t_minishell *minishell, t_pipe *pipe)
{
	// int		i;

	// i = 0;
	(void)pipe;
	// if (!minishell->exec.envp || !minishell->exec.envp[0])
	// 	return (0);
	// i = strv_searchindex(minishell->exec.envp, "PWD=");
	// if (i != -1)
	// {
		printf("%s\n", &minishell->builtin.pwd.result[4]);

	// if (minishell->builtin.pwd.result)
	// 	ft_printf_fd(1, "%s\n", &minishell->builtin.pwd.result[4]);
	// }
	return (0);
}

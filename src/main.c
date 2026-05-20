/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/20 16:01:59 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argc;
	(void)argv;
	ft_bzero(&minishell, sizeof(t_minishell));
	ft_bzero(&minishell.exec, sizeof(t_exec));
	minishell.exec.first_token = &minishell.token;
	strv_dup(&minishell, &minishell.exec.envp, envp);
	init_pwd_envp(&minishell);
	if (execute(&minishell) == 1)
		return (minishell.exec.error);
	return (minishell.exec.error);
}

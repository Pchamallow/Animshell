/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 17:03:23 by pswirgie         ###   ########.fr       */
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
	minishell.exec.envp = strv_dup(&minishell, envp);
	minishell.exec.error_last_prompt = 0;
	init_pwd_envp(&minishell);
	execute(&minishell);
	return (minishell.exec.error);
}

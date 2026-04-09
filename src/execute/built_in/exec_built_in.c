/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:27 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 10:06:06 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_built_in(t_minishell *minishell, t_token *token)
{
	// (void)minishell;
	// ft_printf_fd(2, "token : %s\n", token->value);
	int	len;

	len = ft_strlen(token->value);
	if (ft_strncmp(token->value, "echo", len) == 0)
		minishell->exec.built_in = IS_ECHO;
	else if (ft_strncmp(token->value, "cd", len) == 0)
		minishell->exec.built_in = CD;
	else if (ft_strncmp(token->value, "pwd", len) == 0)
		minishell->exec.built_in = PWD;
	else if (ft_strncmp(token->value, "export", len) == 0)
		minishell->exec.built_in = EXPORT;
	else if (ft_strncmp(token->value, "unset", len) == 0)
		minishell->exec.built_in = UNSET;
	else if (ft_strncmp(token->value, "env", len) == 0)
		minishell->exec.built_in = ENV;
	else if (ft_strncmp(token->value, "exit", len) == 0)
		minishell->exec.built_in = EXIT;
}
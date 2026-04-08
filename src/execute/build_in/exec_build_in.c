/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_build_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:27 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/08 21:21:11 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_build_in(t_minishell *minishell, t_token *token)
{
	if (token->value == "echo")
		minishell->exec.built_in = 1;
	// else if (token->value == "cd")
		// minishell->exec.built_in = 2;
	// else if (token->value == "pwd")
		// minishell->exec.built_in = 3;
	// else if (token->value == "export")
		// minishell->exec.built_in = 4;
	// else if (token->value == "unset")
		// minishell->exec.built_in = 5;
	// else if (token->value == "env")
		// minishell->exec.built_in = 6;
	// else if (token->value == "exit")
		// minishell->exec.built_in = 7;
	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:27 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/20 17:01:35 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_built_in(t_pipe *pipe, t_token *token)
{
	// ft_printf_fd(2, "token : %s\n", token->value);
	int	len;

	len = ft_strlen(token->value);
	if (ft_strncmp(token->value, "echo", len) == 0)
		pipe->builtin_kind = IS_ECHO;
	else if (ft_strncmp(token->value, "cd", len) == 0)
		pipe->builtin_kind = CD;
	else if (ft_strncmp(token->value, "pwd", len) == 0)
		pipe->builtin_kind = PWD;
	else if (ft_strncmp(token->value, "export", len) == 0)
		pipe->builtin_kind = EXPORT;
	else if (ft_strncmp(token->value, "unset", len) == 0)
		pipe->builtin_kind = UNSET;
	else if (ft_strncmp(token->value, "env", len) == 0)
		pipe->builtin_kind = ENV;
	else if (ft_strncmp(token->value, "exit", len) == 0)
		pipe->builtin_kind = EXIT;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_explicit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:11:20 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 11:22:22 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_explicit(t_minishell *minishell, t_token *token)
{
	int	len;

	len = ft_strlen(token->value);
	token->cmd_path = ft_calloc(sizeof(char *), len + 1);
	if (!token->cmd_path)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(token->cmd_path, token->value, len + 1);
	cmd_explicit(minishell, token);
}

/*
if it is an explicit path in token->value, 
"./value" -> "value",
for execve we need only the cmd
*/
void	cmd_explicit(t_minishell *minishell, t_token *token)
{
	int		len;

	len = ft_strlen(token->cmd_path) - 2;
	free(token->value);
	token->value = ft_calloc(sizeof(char *), len + 1);
	if (!token->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(token->value, &token->cmd_path[2], len + 1);
}

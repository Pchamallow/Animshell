/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_explicit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:11:20 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 17:30:59 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
if its a explicit path in token->value, 
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

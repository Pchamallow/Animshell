/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:38:47 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/12 19:59:36 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_args(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	t_token *cmd;
	int	len;
	int	index;
	
	cmd = pipe->cmd;
	len = ft_strlen(token->value) + 1;
	index = pipe->nb_args++;
	cmd->cmd_args[index] = ft_calloc(len, sizeof(char));
	if (!cmd->cmd_args[index])
		print_error_free(minishell, "Error\nMalloc options failed.\n", 1);
	ft_strlcpy(cmd->cmd_args[index], token->value, len);
}

void	init_cmd_args(t_minishell *minishell, t_pipe *pipe, int nb_args)
{
	t_token *cmd;

	cmd = pipe->cmd;
	// pipe->nb_args = 0;
	// ft_printf_fd(2, "test : %s\n", cmd->value);
	// ft_printf_fd(2, "nb_args : %d\n", nb_args);
	cmd->cmd_args = malloc(sizeof(char *) * (nb_args + 1));
	if (!cmd->cmd_args)
		print_error_free(minishell, "Error\nMalloc options failed.\n", 1);
	cmd->cmd_args[nb_args] = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:38:47 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/18 08:55:25 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_args(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	t_token *cmd;
	int	index;
	
	cmd = pipe->cmd;
	index = pipe->nb_args++;
	cmd->cmd_args[index] = ft_strdup(token->value);
	if (!cmd->cmd_args[index])
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	// ft_printf_fd(2, "args cpy : %s\n", cmd->cmd_args[index]);
	// ft_printf_fd(2, "--------------------------------\n");

}

void	init_cmd_args(t_minishell *minishell, t_pipe *pipe, int nb_args)
{
	t_token	*cmd;
	int		i;

	i = 0;
	cmd = pipe->cmd;
	// ft_printf_fd(2, "cmd : %s\n", cmd->value);
	// ft_printf_fd(2, "nb_args : %d\n", nb_args);
	cmd->cmd_args = malloc(sizeof(char *) * (nb_args + 1));
	if (!cmd->cmd_args)
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	while (i <= nb_args)
	{
		cmd->cmd_args[i] = NULL;
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:11:35 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/25 15:31:42 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_line_and_token_list(char *line, t_token **token_list)
{
	free(line);
	ft_token_lstclear(token_list);
}

void	error_malloc(char *line, char *err_msg)//ajouter free envp copy
{
	ft_printf_fd(2, "minishell: ");
	ft_printf_fd(2, "%s", err_msg);
	ft_printf_fd(2, "\n");
	free(line);
	rl_clear_history();
	exit(EXIT_FAILURE);
}

void	error_quote(char *line, t_token **token_list, t_minishell *minishell)
{
	ft_printf_fd(2, "minishell: ");
	ft_printf_fd(2, "error unclosed quotes");
	ft_printf_fd(2, "\n");
	free_line_and_token_list(line, token_list);
	minishell->exec.error = 2;
}

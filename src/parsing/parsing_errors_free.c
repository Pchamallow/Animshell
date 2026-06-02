/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 19:11:35 by stkloutz          #+#    #+#             */
/*   Updated: 2026/06/02 12:04:07 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_line_and_token_list(char *line, t_token **token_list)
{
	free(line);
	ft_token_lstclear(token_list);
}

void	error_malloc(char *line, char *newline,
		t_minishell *minishell, char *err_msg)
{
	ft_printf_fd(2, "minishell: malloc error in ");
	ft_printf_fd(2, "%s\n", err_msg);
	free(line);
	free(newline);
	if (minishell->builtin.echo.result)
		free(minishell->builtin.echo.result);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	if (minishell->builtin.cd.result)
		free(minishell->builtin.cd.result);
	if (minishell->exec.envp)
		free_strv(minishell->exec.envp);
	if (minishell->exec.paths_for_search_cmd)
		free_strv(minishell->exec.paths_for_search_cmd);
	if (minishell->token)
		ft_token_lstclear(minishell->exec.first_token);
	if (minishell->exec.pipe_lst)
		lst_pipe_clear(&minishell->exec.pipe_lst);
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

void	print_error_unexpected_token(t_token *token)
{
	ft_printf_fd(2, "minishell: ");
	ft_printf_fd(2, "syntax error near unexpected token ");
	ft_printf_fd(2, "\'%s\'", token->value);
	ft_printf_fd(2, "\n");
}

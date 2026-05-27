/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:35:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 12:30:28 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strerror_free_structure(t_minishell *minishell, char *filename,
	int error)
{
	char	*err;

	err = strerror(errno);
	write(2, err, strlen(err));
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
	free_all(minishell);
	exit(error);
}

void	error_cmd_args(char *cmd, char *filename, char *error)
{
	if (filename)
		ft_printf_fd(2, "minishell: %s: %s: %s\n", cmd, filename, error);
	else
		ft_printf_fd(2, "minishell: %s: %s\n", cmd, error);
}

void	strerror_file(char *filename)
{
	char	*err;

	err = strerror(errno);
	ft_printf_fd(2, "minishell: ");
	ft_printf_fd(2, "%s: ", filename);
	ft_printf_fd(2, "%s\n", err);
}

void	print_error_free(t_minishell *minishell, char *str, int error)
{
	ft_printf_fd(2, "minishell: ");
	ft_printf_fd(2, "%s", str);
	free_all(minishell);
	exit(error);
}

void	error_free_parsing(t_minishell *minishell)
{
	if (minishell->exec.envp)
		free_strv(minishell->exec.envp);
	if (minishell->exec.paths_for_search_cmd)
		free_strv(minishell->exec.paths_for_search_cmd);
	if (minishell->token)
		ft_token_lstclear(minishell->exec.first_token);
	if (minishell->exec.pipe_lst)
		lst_pipe_clear(&minishell->exec.pipe_lst);
	if (minishell->prompt)
		free(minishell->prompt);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	ft_printf_fd(2, "minishell: Malloc failed.\n");
	exit(EXIT_FAILURE);
}

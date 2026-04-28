/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:35:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/27 15:19:06 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strv(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	lst_pipe_clear(t_pipe **head)
{
	t_pipe	*current;
	t_pipe	*next;

	if (!head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}

void	free_builtin(t_minishell *minishell)
{
	if (minishell->builtin.echo.result)
		free(minishell->builtin.echo.result);
	// minishell->builtin.echo.for_prompt = false;
}

void	free_all(t_minishell *minishell)
{
	if (minishell->exec.envp)
		free_strv(minishell->exec.envp);
	if (minishell->exec.paths_for_search_cmd)
		free_strv(minishell->exec.paths_for_search_cmd);
	free_builtin(minishell);
	if (minishell->token)
		ft_token_lstclear(minishell->exec.first_token);
	if (minishell->exec.pipe_lst)
		lst_pipe_clear(&minishell->exec.pipe_lst);
	free(minishell->prompt);
	// print_pauline(minishell);// print la commande et les arguments
}

void	strerror_free_structure(t_minishell *minishell, char *filename, int error)
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

void	error_cmd_args(t_minishell *minishell, char *cmd, char *filename)
{
	/*    UTILISER ? */
	ft_printf_fd(2, "%s", cmd);
	// ft_printf_fd(2, "NOPE");
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
	free_all(minishell);
	exit(minishell->exec.error);
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

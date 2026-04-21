/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:35:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/21 16:40:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// void	free_structure(t_minishell *minishell)
// {
// 	t_token *token;

// 	token = &minishell->token;
// 	while (token.value != NULL)
// 	{
		
// 	}
// 	if (type->close == 0)
// 		close(type->fd);
// 	if (type->options)
// 		free_double(type->options, type->nb_opt);
// 	if (type->cmd_path)
// 		free(type->cmd_path);
// 	if (type->args_execve)
// 		free_double(type->args_execve, type->nb_opt + 1);
// 	if (i == 1)
// 		return ;
// 	free_structure(&parse->out, parse, 1);
// }

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
void	free_all(t_minishell *minishell)
{
	ft_token_lstclear(minishell->exec.first_token);
	lst_pipe_clear(&minishell->exec.pipe_lst);
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
	// ft_token_lstclear(head);
    (void)minishell;
	ft_printf_fd(2, "%s", str);
	exit(error);
}

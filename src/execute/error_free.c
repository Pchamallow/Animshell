/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:35:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/10 12:15:55 by pswirgie         ###   ########.fr       */
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

void	strerror_free_structure(t_minishell *minishell, char *filename, int error)
{
	char	*err;

	err = strerror(errno);
	write(2, err, strlen(err));
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
	tmp_free(minishell);
	exit(error);
}

void	strerror_print(char *filename)
{
	char	*err;

	err = strerror(errno);
	write(2, err, strlen(err));
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
}

void	print_error_free(t_minishell *minishell, char *str, int error)
{
	// ft_token_lstclear(head);
    (void)minishell;
	ft_printf_fd(2, "%s", str);
	exit(error);
}

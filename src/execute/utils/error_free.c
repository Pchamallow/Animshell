/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:35:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 15:33:12 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	error_cmd_args(char *cmd, char *filename, char *error)
{
	/*    UTILISER ? */
	if (filename)
		ft_printf_fd(2, "minishell: %s: %s: %s\n", cmd, filename, error);
	else
		ft_printf_fd(2, "minishell: %s: %s\n", cmd, error);
	// ft_printf_fd(2, "NOPE");
	// free_all(minishell);
	// exit(minishell->exec.error);
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

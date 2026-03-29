/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 10:55:54 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/24 11:16:32 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_double(char **tab, int len)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (i < len)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_structure(t_element *type, t_parse *parse, int i)
{
	if (type->close == 0)
		close(type->fd);
	if (type->options)
		free_double(type->options, type->nb_opt);
	if (type->cmd)
		free(type->cmd);
	if (type->cmd_path)
		free(type->cmd_path);
	if (type->args_execve)
		free_double(type->args_execve, type->nb_opt + 1);
	if (i == 1)
		return ;
	free_structure(&parse->out, parse, 1);
}

void	strerror_free_structure(t_parse *parse, char *filename, int error)
{
	char	*err;

	err = strerror(errno);
	write(2, err, strlen(err));
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
	free_structure(&parse->in, parse, 0);
	exit(error);
}

void	print_error_free(t_parse *parse, char *str, int error)
{
	free_structure(&parse->in, parse, 0);
	ft_printf_fd(2, "%s", str);
	exit(error);
}

void	print_error(char *str, int error)
{
	ft_printf_fd(2, "%s", str);
	exit(error);
}

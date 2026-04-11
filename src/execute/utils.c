/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:22 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/11 20:53:03 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_minishell *minishell)
{
	t_pipe *line;

	line = minishell->exec.pipe_a;
	if (line->infile)
	{
		// printf("ICI\n");
		// ft_printf_fd(2, "FD : %d\n", line->infile->fd);
		ft_printf_fd(2, "infile  : %s\n", line->infile->value);
		// ft_printf_fd(2, "outfile : %s\n", line->outfile->value);
		close(line->infile->fd);
		// ft_printf_fd(2, "FD : %d\n", line->infile->fd);
	}
	if (line->outfile)
		close(line->outfile->fd);
}

// /* cmd without spaces */
// int	len_cmd(char *str)
// {
// 	int	i;
// 	int	cmd;

// 	i = 0;
// 	cmd = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != ' ')
// 		{
// 			cmd = 1;
// 			while (str[i] && str[i] != ' ')
// 				i++;
// 			if (str[i] == ' ')
// 			{
// 				while (str[i] && str[i] == ' ')
// 					i++;
// 			}
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (cmd == 0)
// 		return (0);
// 	return (i);
// }

int	len_double(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	if (i != 0)
		i--;
	return (i);
}

int	len_cmd_no_endspace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			while (str[i] && str[i] != ' ')
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	is_sign(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == '-')
		return (1);
	return (0);
}

int	is_space(char *str, int not)
{
	int	i;

	i = 0;
	if (not == 1)
	{
		while (str[i] && str[i] != ' ')
			i++;
	}
	else
	{
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (i);
}

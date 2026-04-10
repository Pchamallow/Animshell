/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:22 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/10 14:16:18 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_minishell *minishell)
{
	t_pipe *line;

	line = minishell->exec.pipe_a;
	if (line->infile->close == 0)
	{
		close(line->infile->fd);
		line->infile->close = 1;
	}
	if (line->outfile)
	{
		close(line->outfile->fd);
		line->outfile->close = 1;
	}
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

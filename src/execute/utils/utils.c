/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:22 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/24 17:23:25 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lst_size(t_token *token)
{
	int	count;

	count = 0;
	if (!token)
		return (0);
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

void	close_fds_pipe(t_pipe *pipe)
{
	if (pipe->infile && pipe->infile->fd >= 0)
		close(pipe->infile->fd);
	// else
		// printf("--------------- already closed infile\n");
	if (pipe->outfile && pipe->outfile->fd >= 0)
	{
		// printf("--------------- close outfile\n");
		close(pipe->outfile->fd);
	}
	// else
		// printf("---------- already closed infile\n");
}

void	close_fd(int fd)
{
	if (fd && fd >= 0)
		close(fd);
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

bool find_built_in(char *token)
{
	const char *builtins[7];
	int	i;
	int	len;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	len = ft_strlen(token);

	while (i < 7)
	{
		if (ft_strncmp(token, builtins[i], len) == 0)
			return (true);
		i++;
	}
	return (false);
}
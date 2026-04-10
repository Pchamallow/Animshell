/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:22 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/06 20:41:39 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	close_fds(t_parse *parse)
// {
// 	if (parse->in.close == 0)
// 	{
// 		close(parse->in.fd);
// 		parse->in.close = 1;
// 	}
// 	if (parse->out.close == 0)
// 	{
// 		close(parse->out.fd);
// 		parse->out.close = 1;
// 	}
// }

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
	/*int	cmd;*/

	i = 0;
	/*cmd = 0;*/
	while (str[i])
	{
		if (str[i] != ' ')
		{
			/*cmd = 1;*/
			while (str[i] && str[i] != ' ')
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

// int	is_sign(char c)
// {
// 	if (c == '<' || c == '>' || c == '|' || c == '&' || c == '-')
// 		return (1);
// 	return (0);
// }

// int	is_space(char *str, int not)
// {
// 	int	i;

// 	i = 0;
// 	if (not == 1)
// 	{
// 		while (str[i] && str[i] != ' ')
// 			i++;
// 	}
// 	else
// 	{
// 		while (str[i] && str[i] == ' ')
// 			i++;
// 	}
// 	return (i);
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/25 16:51:49 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int index_word(char *src)
{
	char	*echo = "echo ";
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i + j] == echo[j])
			j++;
		else
		{
			j = 0;
			i++;
		}
		if (!echo[j])
			return (i + j);
	}
	return (0);
}

// ECHO ****************
/*- print a given string
conditions

- exit status to print
- - if $? is find, 
- - if $?word or word$? 
- - no print with $ ?*/
// *********************
int echo(t_minishell *minishell, t_pipe *pipe)
{
	t_token *args;
	int		i;
	int		j;
	int		len;

	i = 0;
	(void)pipe;
	args = NULL;
	if (pipe->cmd->next)
		args = pipe->cmd->next;
	else
		return (0);
	// args = args->next;
	while (args && (minishell->exec.index_pipe == 0 || i < minishell->exec.index_pipe))
	{
		len = ft_strlen(args->value);
		if (ft_strnstr(args->value, "$?", len + 1) != NULL)
		{
			j = 0;
			while (args->value[j])
			{
				if (args->value[j] == '$' && args->value[j + 1] == '?')
				{
					ft_printf_fd(1, "%d", minishell->exec.error);
					j += 2;
					continue;
				}
				ft_printf_fd(1, "%c", args->value[j]);
				j++;
			}
		}
		else if (args->type == IS_INPUT)
			return (0);
		else
			ft_printf_fd(1, "%s", args->value);
		// ft_printf_fd(2, "%d", args->type);
		args = args->next;
		i++;
	}
	ft_printf_fd(1, "\n");
	return (0);
}

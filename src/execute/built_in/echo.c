/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/13 19:16:48 by pswirgie         ###   ########.fr       */
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
int echo(t_minishell *minishell)
{
	t_token *args;
	int		i;
	int		j;
	int		len;

	i = 0;
	args = minishell->exec.pipe_lst->cmd;
	args = args->next;
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
		else
			ft_printf_fd(1, "%s", args->value);
		// ft_printf_fd(2, "%d", args->type);
		args = args->next;
		i++;
	}
	ft_printf_fd(1, "\n");
	return (0);
}

/*
faire la verification de cmd, voir si elle passe,
si el chemin est explicite ou non etc
-> test des cmd, voir si word = echo 
-> exec.echo = 1 si il y a "echo " + un mot derriere
voir les diffrents contextes d utilisation d echo 

parsing verifier les mots les uns apres les autres
que ce soit bien pas des commandes


j ai besoin dune structure ? ou pas 

mot1

cmd1

mot2

cmd2


ici pour dire a echo print les mots d apres
mais arrete s il autre chose qu un mot
= recuperer le type

*/